#include <stdio.h>
#include "Path.h"

Path::~Path(){
    printf("Destrutor da classe Path atuando.\n");
}

Path::Path(vector<City*> cities){
    this->cities = cities;
    this->size = cities.size();
    initCost();

    printf("Criando caminho de tamanho %d e custo inicial %f\n", this->size, this->cost);
}

void Path::initCost(){
    cost = 0.0;

    for (size_t i = 0; i < size; i++)
    {
        cost+= cities[i]->distance(cities[(i+1)%size]);
    }

    printf("Preco calculado %f\n\n", cost);
}

float Path::distNext(int i){
    return cities[i]->distance(cities[(i+1)%size]);
}

float Path::distPrevious(int i){
    return cities[i]->distance(cities[(i+size-1)%size]);
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

