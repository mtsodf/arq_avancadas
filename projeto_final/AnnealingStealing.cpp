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

AnnealingStealing::AnnealingStealing(Path *path, float temperature){
    this->path = path;
    this->temperature = temperature;
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
    temperature *= 0.9;
}

void AnnealingStealing::solve(){

    int iters=0;
    do{
        printf("Temperatura atual %f. Custo: %f\n", this->temperature, path->cost);
        trySwap();
        iters++;
        //if(iters%100 == 0){

        //}
    } while(temperature > 1e-6);

}

