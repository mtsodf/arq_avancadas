#include "AnnealingStealing.h"
#include "Path.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

using namespace std;

void sortSwap(int *i, int *j, int n){
    *i = rand() % n;
    *j = rand() % n;
}

float rand_float(){
    return (float)rand()/(float)RAND_MAX;
}

AnnealingStealing::AnnealingStealing(Path *path, float temperature, float alpha){
    this->path = path;
    this->temperature = temperature;
    this->alpha = alpha;
}

float AnnealingStealing::trySwap(){
    int i, j;
    float cost;

    cost = path->cost;

    sortSwap(&i, &j, path->size);

    path->swap(i,j);

    float delta  = path->cost - cost;


    if(delta < 0){
        updateTemperature();
        return path->cost;
    }else{
        if(exp(-delta/temperature) > rand_float()){
            updateTemperature();
            return path->cost;
        } else{
            path->swap(i,j);
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

    int iters=0;
    do{
        if(log) printf("Temperatura atual %f. Custo: %f\n", this->temperature, path->cost);
        trySwap();
        iters++;
    } while(temperature > 1e-6);

}

