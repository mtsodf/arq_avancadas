#include "Opt2.h"
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <mpi.h>
#include "utils.h"
#define epsilon -1e-12


void findMiniMinjOpenMP(Path* path, int *mini, int *minj, double* minchange, double* times){

    *minchange = 0.0;

    int num_threads;

    #pragma omp parallel shared(times)
    {
        int id = omp_get_thread_num();
        getTimeCounter(id)->startTimer(findMinimumSection);

        int inext, jnext;
        float change;

        num_threads = omp_get_num_threads();
        int mini_local, minj_local;
        double minchange_local = 0.0;
        double timeNow;

        if(id==0)  times[0] = get_clock_msec();

        for(int i = id; i < path->size-2;i+=num_threads){
            inext = (i+1)%(path->size);
            for(int j = i+2; j < path->size; j++){
                jnext = (j+1)%(path->size);
                change = path->dist(i,j) + path->dist(inext, jnext) - path->dist(i,inext) - path->dist(j, jnext);
                if(minchange_local > change){
                    minchange_local = change;
                    mini_local = i;
                    minj_local = j;
                }
            }
        }

        getTimeCounter(id)->endStartTimer(findMinimumSection, barrierSection);

        if(id==0) times[1] = get_clock_msec();

        #pragma omp critical
        {
            //printf("RODANDO %d\n", id);
            if(minchange_local < *minchange){
                *minchange = minchange_local;
                *mini = mini_local;
                *minj = minj_local;
            }
        }

        if(id==0) times[2] = get_clock_msec();
        getTimeCounter(id)->startTimer(endParallelSection);
    }

    double timeNow = get_clock_msec();
    for (size_t i = 0; i < num_threads; i++)
    {
        getTimeCounter(i)->endTimer(barrierSection), timeNow;
        getTimeCounter(i)->endTimer(endParallelSection, timeNow);
    }
}


void findMiniMinjOpenMPNew(Path* path, int *mini, int *minj, double* minchange){

    int inext, jnext;
    double change;
    int mini_local, minj_local;
    double minchange_local = 0.0, timeNow;


    int id = omp_get_thread_num();
    int num_threads = omp_get_num_threads();
    getTimeCounter(id)->startTimer(findMinimumSection);


    for(int i = id; i < path->size-2;i+=num_threads){
        inext = (i+1)%(path->size);
        for(int j = i+2; j < path->size; j++){
            jnext = (j+1)%(path->size);
            change = path->dist(i,j) + path->dist(inext, jnext) - path->dist(i,inext) - path->dist(j, jnext);
            if(minchange_local > change){
                minchange_local = change;
                mini_local = i;
                minj_local = j;
            }
        }
    }

    timeNow = get_clock_msec();
    getTimeCounter(id)->endTimer(findMinimumSection, timeNow);
    getTimeCounter(id)->startTimer(barrierSection, timeNow);
    getTimeCounter(id)->startTimer(criticalSection, timeNow);

    #pragma omp critical
    {
        if(minchange_local < *minchange){
            *minchange = minchange_local;
            *mini = mini_local;
            *minj = minj_local;
        }
    }

    getTimeCounter(id)->endTimer(criticalSection);
    #pragma omp barrier
}


void findMiniMinjOpenMP(Path* path, int *mini, int *minj, double* minchange){
    double aux[3];
    findMiniMinjOpenMP(path, mini, minj, minchange, aux);
}


Opt2::Opt2(Path* path){
    this->path = path;
}


bool Opt2::solveOpenMP(Path* path, bool log, int *iters, bool logAllPaths){

    double minchange = 0.0;
    int mini, minj;
    *iters = 0;
    bool changed = false;
    int num_threads;

    #pragma omp parallel shared(mini, minj, minchange, path, changed, num_threads)
    {
        int id = omp_get_thread_num();
        num_threads = omp_get_num_threads();
        do{

            findMiniMinjOpenMPNew(path, &mini, &minj, &minchange);

            getTimeCounter(id)->startTimer(swapSection);
            if(id == 0)
            {
                if(logAllPaths){
                    char filename[60];
                    sprintf(filename, "path_%.2d.txt", *iters);
                    PrintPath(filename, path);
                    FILE* out = fopen(filename, "a+");
                    fprintf(out, "%d %d\n", mini, minj);
                    fclose(out);
                }

                if(minchange < epsilon) {
                    path->swapTotal(mini+1, minj);
                    changed = true;
                } else{
                    changed = false;
                }

                (*iters)++;
                minchange = 0.0;

            }
            getTimeCounter(id)->endTimer(swapSection);
            #pragma omp barrier
            getTimeCounter(id)->endTimer(barrierSection);

        } while(changed);

        getTimeCounter(id)->startTimer(endParallelSection);
    }
    double timeNow = get_clock_msec();
    for (size_t i = 0; i < num_threads; i++)
    {
        getTimeCounter(i)->endTimer(endParallelSection, timeNow);
    }


}


bool Opt2::solve(Path* path, bool log, int *iters, double *times, bool logAllPaths){

    double minchange = 0.0, start, end;
    int mini, minj;
    *iters = 0;
    double times_find[3];
    times[0] = 0.0;
    times[1] = 0.0;
    start = get_clock_msec();
    do{
        findMiniMinjOpenMP(path, &mini, &minj, &minchange, times_find);
        //printf("%5d MINCHANGE = %10lf (%5d, %5d)\n", *iters, minchange, mini, minj);

        getTimeCounter(0)->startTimer(swapSection);
        if(minchange < epsilon)  path->swapTotal(mini+1, minj);
        getTimeCounter(0)->endTimer(swapSection);

        (*iters)++;
        //if((*iters)%100==0 && log) printf("Iteracao %d. Custo %f\n", iters, path->cost);
        times[0] += times_find[1] - times_find[0];
        times[1] += times_find[2] - times_find[1];

        if(log){
            char filename[60];
            sprintf(filename, "path_%d.txt", *iters);
            FILE* out = fopen(filename, "w");

            fclose(out);
        }
    } while(minchange < epsilon);

}

bool Opt2::solve(Path* path, bool log){
    double aux[2];
    int aux_int;
    solve(path, log, &aux_int, aux, false);
}

void findMiniMinjMPI(Path* path, int *mini, int *minj, double* minchange){

    *minchange = 0.0;
    double * minchange_array;

    int inext, jnext;
    double change;

    int rank, mpi_size;


    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    minchange_array = (double*)malloc(sizeof(double)*mpi_size);

    getTimeCounter(rank)->startTimer(findMinimumSection);

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

    getTimeCounter(rank)->endStartTimer(findMinimumSection, barrierSection);

    MPI_Allgather(minchange, 1, MPI_DOUBLE, minchange_array, 1, MPI_DOUBLE, MPI_COMM_WORLD);

    int min_rank = 0; *minchange = minchange_array[0];
    for(int i = 1; i < mpi_size; i++){
        if(*minchange > minchange_array[i]){
            *minchange = minchange_array[i];
            min_rank = i;
        }
    }

    MPI_Bcast(mini, 1, MPI_INT, min_rank, MPI_COMM_WORLD);
    MPI_Bcast(minj, 1, MPI_INT, min_rank, MPI_COMM_WORLD);
}

bool Opt2::solveMPI(Path *path, bool log, int* iters, double *times, bool logAllPaths){

    double minchange = 0.0;
    int mini, minj;
    *iters = 0;

    int rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    do{
        findMiniMinjMPI(path, &mini, &minj, &minchange);
        //if(rank == 0) printf("%5d MINCHANGE = %10lf (%5d, %5d)\n", *iters, minchange, mini, minj);

        getTimeCounter(rank)->startTimer(swapSection);
        if(minchange < epsilon)  path->swapTotal(mini+1, minj);
        getTimeCounter(rank)->endTimer(swapSection);
        (*iters)++;

        getTimeCounter(rank)->endTimer(barrierSection);

        if((*iters)%100==0 && rank == 0) printf("Iteracao %d. Custo %lf. %d %d\n", *iters, path->cost, mini, minj);
    } while(minchange < epsilon);

}
