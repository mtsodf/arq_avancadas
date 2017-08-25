#include <stdio.h>
#include <stdlib.h>
#include "Path.h"

Path::~Path(){
    printf("Destrutor da classe Path atuando.\n");
}

Path::Path(vector<City*> cities){
    this->cities = cities;
    this->size = cities.size();
    initCost();

    // printf("Criando caminho de tamanho %d e custo inicial %f\n", this->size, this->cost);
}

void Path::initCost(){
    cost = 0.0;

    for (size_t i = 0; i < size; i++)
    {
        cost+= cities[i]->distance(cities[(i+1)%size]);
    }

}

float Path::distNext(int i){
    return cities[i]->distance(cities[(i+1)%size]);
}

float Path::distPrevious(int i){
    return cities[i]->distance(cities[(i+size-1)%size]);
}

Path* Path::copy(){
    vector<City*> cities(this->size);

    for (size_t i = 0; i < this->size; i++)
    {
        cities[i] = this->cities[i];
    }

    return new Path(cities);
}

float Path::swap(int i, int j){
    City *aux;

    cost -= distNext(i);
    cost -= distPrevious(i);

    cost -= distNext(j);
    cost -= distPrevious(j);

    aux = cities[i];
    cities[i]= cities[j];
    cities[j] = aux;

    cost += distNext(i);
    cost += distPrevious(i);

    cost += distNext(j);
    cost += distPrevious(j);

    return cost;
}

float Path::swapTotal(int i, int j){
    City *aux;

    if(i > j){
        int a = i;
        i = j;
        j = a;
    }

    for (size_t k = 0; k <= (j-i)/2; k++)
    {
        aux = cities[i+k];
        cities[i+k] = cities[j-k];
        cities[j-k] = aux;
    }

    initCost();

    return cost;
}

void Path::scramble(){
    City *aux;
    int j;
    for(int i = 1; i < this->size;i++){
        j = rand() % (i+1);

        aux = this->cities[i];
        this->cities[i] = this->cities[j];
        this->cities[j] = aux;
    }
    initCost();
}