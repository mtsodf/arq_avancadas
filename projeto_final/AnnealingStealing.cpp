#include "AnnealingStealing.h"
#include "Path.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

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
    solve(log, 0, 1e20);
}

void AnnealingStealing::solve(bool log, int min_iters, int max_iters){
    FILE* out;
    if(log) out = fopen("out.txt", "w");
    int iters=0;
    do{
        if(log && iters%100 == 0) fprintf(out, "%d %e %e\n", iters, path->cost, temperature);
        trySwap();
        iters++;
    } while((temperature > limit || iters < min_iters) && iters < max_iters);

    if(log) PrintPath("path.txt", path);

    printf("Numero de iteracoes %d\n", iters);
}



