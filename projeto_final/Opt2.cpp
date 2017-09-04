#include "Opt2.h"
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <mpi.h>
#include "utils.h"
#define epsilon -1e-12


double minchanges[MAX_THREADS];
int minis[MAX_THREADS], minjs[MAX_THREADS];

Opt2::Opt2(Path* path){
    this->path = path;
}

void findMiniMinjOpenMP(Path* path, int *mini, int *minj, double* minchange){

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

            findMiniMinjOpenMP(path, &mini, &minj, &minchange);

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


void findMiniMinjOpenMPWhitoutCritical(Path* path, int *mini, int *minj, double* minchange){

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

    #pragma omp barrier
    minchanges[id] = minchange_local;
    minis[id] = mini_local;
    minjs[id] = minj_local;
    #pragma omp barrier

    timeNow = get_clock_msec();
    getTimeCounter(id)->endTimer(findMinimumSection, timeNow);

    getTimeCounter(id)->startTimer(barrierSection, timeNow);



    *minchange = minchanges[0];
    *mini = minis[0];
    *minj = minjs[0];
    for (size_t i = 1; i < num_threads; i++)
    {
        if(minchanges[i] < *minchange){
            *minchange = minchanges[i];
            *mini = minis[i];
            *minj = minjs[i];
        }
    }

}


bool Opt2::solveOpenMPWhitoutCritical(Path* path, bool log, int *iters, bool logAllPaths){


    *iters = 0;
    int num_threads;
    int iters_private = 0;
    #pragma omp parallel shared(path, num_threads) firstprivate(iters_private)
    {
        int id = omp_get_thread_num();
        num_threads = omp_get_num_threads();
        bool changed = false;
        int mini, minj;
        double minchange = 0.0;



        #pragma omp single
        {
            copyPaths(num_threads);
        }

        iters_private = 0;

        do{
            //printf("PATHS VALUES (%d) %f\n", id, paths[id]->cost);
            findMiniMinjOpenMPWhitoutCritical(paths[id], &mini, &minj, &minchange);

            //printf("MINI MINJ (%d) %d %d\n", id, mini, minj);
            getTimeCounter(id)->startTimer(swapSection);

            if(minchange < epsilon) {
                paths[id]->swapTotal(mini+1, minj);
                changed = true;
            } else{
                changed = false;
            }

            iters_private++;
            minchange = 0.0;

            getTimeCounter(id)->endTimer(swapSection);
            getTimeCounter(id)->endTimer(barrierSection);

        } while(changed);

        getTimeCounter(id)->startTimer(endParallelSection);
    }

    double timeNow = get_clock_msec();
    for (size_t i = 0; i < num_threads; i++)
    {
        getTimeCounter(i)->endTimer(endParallelSection, timeNow);
    }

    *iters = iters_private;

}



void findMiniMinjOpenMPStatic(Path* path, int *mini, int *minj, double* minchange){

    *minchange = 0.0;

    #pragma omp parallel
    {
        int mini_local, minj_local;
        double minchange_local = 0.0;
        int inext, jnext;
        double change;
        int id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        getTimeCounter(id)->startTimer(findMinimumSection);
        #pragma omp for schedule(static) nowait
        for(int i = 0; i < path->size-2;i++){
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
        getTimeCounter(id)->startTimer(criticalSection);
        #pragma omp critical
        {
            if(minchange_local < *minchange){
                *minchange = minchange_local;
                *mini = mini_local;
                *minj = minj_local;
            }
        }

    }
}


bool Opt2::solveOpenMPStatic(Path* path, bool log, int *iters, bool logAllPaths){

    double minchange = 0.0;
    int mini, minj;
    *iters = 0;

    do{

        findMiniMinjOpenMPStatic(path, &mini, &minj, &minchange);

        getTimeCounter(0)->startTimer(swapSection);
        if(minchange < epsilon)  path->swapTotal(mini+1, minj);
        getTimeCounter(0)->endTimer(swapSection);
        getTimeCounter(0)->endTimer(barrierSection);

        (*iters)++;

    } while(minchange < epsilon);

}


void findMiniMinjOpenMPDynamic(Path* path, int *mini, int *minj, double* minchange){

    *minchange = 0.0;

    #pragma omp parallel
    {
        int mini_local, minj_local;
        double minchange_local = 0.0;
        int inext, jnext;
        double change;
        int id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        getTimeCounter(id)->startTimer(findMinimumSection);
        #pragma omp for schedule(dynamic, 10) nowait
        for(int i = 0; i < path->size-2;i++){
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
        getTimeCounter(id)->startTimer(criticalSection);
        #pragma omp critical
        {
            if(minchange_local < *minchange){
                *minchange = minchange_local;
                *mini = mini_local;
                *minj = minj_local;
            }
        }

    }
}


bool Opt2::solveOpenMPDynamic(Path* path, bool log, int *iters, bool logAllPaths){

    double minchange = 0.0;
    int mini, minj;
    *iters = 0;

    do{

        findMiniMinjOpenMPDynamic(path, &mini, &minj, &minchange);

        getTimeCounter(0)->startTimer(swapSection);
        if(minchange < epsilon)  path->swapTotal(mini+1, minj);
        getTimeCounter(0)->endTimer(swapSection);
        getTimeCounter(0)->endTimer(barrierSection);

        (*iters)++;

    } while(minchange < epsilon);

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

    getTimeCounter(0)->startTimer(findMinimumSection);

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

    getTimeCounter(0)->endStartTimer(findMinimumSection, barrierSection);

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

        getTimeCounter(0)->startTimer(swapSection);
        if(minchange < epsilon)  path->swapTotal(mini+1, minj);
        getTimeCounter(0)->endTimer(swapSection);
        (*iters)++;

        getTimeCounter(0)->endTimer(barrierSection);

        if((*iters)%100==0 && rank == 0) printf("Iteracao %d. Custo %lf. %d %d\n", *iters, path->cost, mini, minj);
    } while(minchange < epsilon);

}


void Opt2::copyPaths(int qtd){
    for (size_t i = 0; i < qtd; i++)
    {
        paths[i] = path->copy();
    }
}
