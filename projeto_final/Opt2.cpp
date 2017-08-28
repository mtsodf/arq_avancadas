#include "Opt2.h"
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>


void findMiniMinj(Path* path, int *mini, int *minj, float* minchange){

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
        findMiniMinj(path, &mini, &minj, &minchange);
        if(minchange < 0.0)  path->swapTotal(mini+1, minj);
        iters++;
        if(iters%100==0) printf("Iteracao %d. Custo %f\n", iters, path->cost);
    } while(minchange < 0.0);

}