#include "AnnealingStealing.h"
#include "Path.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <omp.h>

using namespace std;

void PrintPath(char* filename, Path* path){
    FILE* out = fopen(filename, "w");

    for (size_t i = 0; i < path->size; i++)
    {
        fprintf(out, "%d %f %f\n", path->cities[i]->id, path->cities[i]->x, path->cities[i]->y);
    }

    fclose(out);
}

void sortSwap(int *i, int *j, int n){
    *i = rand() % n;
    *j = rand() % n;
}

float rand_float(){
    return (float)rand()/(float)RAND_MAX;
}

AnnealingStealing::AnnealingStealing(Path *path, float temperature, float alpha, float limit){
    this->path = path;
    this->temperature = temperature;
    this->alpha = alpha;
    this->limit = limit;
}

float AnnealingStealing::trySwap(){
    int i, j;
    float cost;

    cost = path->cost;

    sortSwap(&i, &j, path->size);

    path->swapTotal(i,j);

    float delta  = path->cost - cost;


    if(delta < 0){
        updateTemperature();
        return path->cost;
    }else{
        if(exp(-delta/temperature) > rand_float()){
            updateTemperature();
            return path->cost;
        } else{
            path->swapTotal(i,j);
            updateTemperature();
            return path->cost;
        }
    }
}


float AnnealingStealing::updateTemperature(){
    temperature *= alpha;
}



void AnnealingStealing::solve(){
    solve(false);
}

void AnnealingStealing::solve(bool log){
    int iters;
    solve(log, 0, 2e9);
}

void AnnealingStealing::solve(bool log, int min_iters, int max_iters){
    solve(log, min_iters, max_iters, 1);
}

void AnnealingStealing::solveOpenMp(bool log, int min_iters, int max_iters, int outer_iters){
    #pragma omp parallel
    {
        printf("Thread %d\n", omp_get_num_threads();
    }\

}

void AnnealingStealing::solve(bool log, int min_iters, int max_iters, int outer_iters){
    FILE* out;
    if(log) out = fopen("out.txt", "w");
    int iters=0;

    float initTemperature = this->temperature;

    for (int i = 0; i < outer_iters; i++)
    {
        this->temperature = initTemperature*pow(0.8, i);
        do{
            if(log && iters%1 == 0) fprintf(out, "%8d %e %e\n", iters, path->cost, temperature);
            trySwap();
            iters++;
        } while((temperature > limit || iters < min_iters) && iters < max_iters);
    }


    if(log) fclose(out);
    if(log) PrintPath("path.txt", path);


    printf("Numero de iteracoes %d\n", iters);
}



