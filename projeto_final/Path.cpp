#include "Path.h"


Path::Path(vector<City*> cities){
    this->cities = cities;
    this->size = cities.size();
    initCost();

    printf("Criando caminho de tamanho %d e custo inicial %f", this->size, this->cost);
}

void Path::initCost(){
    cost = 0.0;

    for (size_t i = 0; i < cities.size(); i++)
    {
        cost+= cities[i]->distance(cities[(i+1)%size]);
    }

}

float Path::distNext(int i){
    return cities[i]->distance(cities[(i+1)%size]);
}

float Path::distPrevious(int i){
    return cities[i]->distance(cities[(i-1)%size]);
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
