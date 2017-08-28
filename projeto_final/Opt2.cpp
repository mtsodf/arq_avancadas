#include "Opt2.h"
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <mpi.h>

void findMiniMinjOpenMP(Path* path, int *mini, int *minj, float* minchange){

    *minchange = 0.0;
    float * minchange_array;
    int *mini_array, *minj_array;
    #pragma omp parallel shared(minchange_array, mini_array, minj_array)
    {
        int inext, jnext;
        float change;
        int id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        #pragma omp single
        {
            minchange_array = (float*)malloc(sizeof(float)*num_threads);
            mini_array = (int*)malloc(sizeof(int)*num_threads);
            minj_array = (int*)malloc(sizeof(int)*num_threads);
            for(int i = 0; i < num_threads; i++) minchange_array[i] = 0.0;
        }

        #pragma omp for schedule(dynamic, 10)
        for(int i = 0; i < path->size-2;i++){
            inext = (i+1)%(path->size);
            for(int j = i+2; j < path->size; j++){
                jnext = (j+1)%(path->size);
                change = path->dist(i,j) + path->dist(inext, jnext) - path->dist(i,inext) - path->dist(j, jnext);
                if(minchange_array[id] > change){
                    minchange_array[id] = change;
                    mini_array[id] = i;
                    minj_array[id] = j;
                }
            }
        }

        #pragma omp single
        {
            for(int i = 0; i < num_threads; i++){
                if(*minchange > minchange_array[i]){
                    *minchange = minchange_array[i];
                    *mini = mini_array[i];
                    *minj = minj_array[i];
                }
            }
            free(minchange_array);
            free(mini_array);
            free(minj_array);
        }
    }
}


Opt2::Opt2(Path* path){
    this->path = path;
}


bool Opt2::solve(Path* path, bool log){

    float minchange = 0.0;
    int mini, minj;
    int iters = 0;

    do{
        findMiniMinjOpenMP(path, &mini, &minj, &minchange);
        if(minchange < 0.0)  path->swapTotal(mini+1, minj);
        iters++;
        if(iters%100==0) printf("Iteracao %d. Custo %f\n", iters, path->cost);
    } while(minchange < 0.0);

}

void findMiniMinjMPI(Path* path, int *mini, int *minj, float* minchange){

    *minchange = 0.0;
    float * minchange_array;

    int inext, jnext;
    float change;

    int rank, mpi_size;


    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    minchange_array = (float*)malloc(sizeof(float)*mpi_size);


    for(int i = rank; i < path->size-2;i=i+mpi_size){
        inext = (i+1)%(path->size);
        for(int j = i+2; j < path->size; j++){
            jnext = (j+1)%(path->size);
            change = path->dist(i,j) + path->dist(inext, jnext) - path->dist(i,inext) - path->dist(j, jnext);
                if(*minchange > change){
                    *minchange = change;
                    *mini = i;
                    *minj = j;
                }
            }
    }

    //printf("FIM ITERACAO RANK=%d %f\n", rank, *minchange);
    MPI_Allgather(minchange, 1, MPI_FLOAT, minchange_array, 1, MPI_FLOAT, MPI_COMM_WORLD);
    //printf("FIM GATHER RANK=%d\n", rank);

    // for(int i = 0; i  < mpi_size; i++){
    //     if(rank == i){
    //         printf("Minchange valor ");
    //         for(int j = 0; j  < mpi_size; j++){
    //             printf("%10.2f", minchange_array[j]);
    //         }
    //         printf("\n");
    //     }
    //     MPI_Barrier(MPI_COMM_WORLD);
    // }

    int min_rank = 0; *minchange = minchange_array[0];
    for(int i = 1; i < mpi_size; i++){
        if(*minchange > minchange_array[i]){
            *minchange = minchange_array[i];
            min_rank = i;
        }
    }

    MPI_Bcast(mini,1,MPI_INT,min_rank,MPI_COMM_WORLD);
    MPI_Bcast(minj,1,MPI_INT,min_rank,MPI_COMM_WORLD);


}

bool Opt2::solveMPI(Path *path, bool log){
    float minchange = 0.0;
    int mini, minj;
    int iters = 0;

    do{
        findMiniMinjMPI(path, &mini, &minj, &minchange);
        if(minchange < 0.0)  path->swapTotal(mini+1, minj);
        iters++;
        if(iters%100==0) printf("Iteracao %d. Custo %f. %d %d\n", iters, path->cost, mini, minj);
    } while(minchange < 0.0);
}