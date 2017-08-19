#include <iostream>
#include <vector>
#include <stdlib.h>
#include "Read.h"
#include "Path.h"
#include "City.h"
#include "AnnealingStealing.h"


using namespace std;

void distributeCities(float*, float*);



int main(){
    float *xs, *ys;
    int n;
    ler_cidades("../ALL_tsp/ch4.tsp", &n, &xs, &ys);


    cout << "Valores das cidades" << endl;

    vector<City*> cities;
    cities.reserve(n);

    distributeCities(xs, ys);

    for (size_t i = 0; i < n; i++)
    {
        printf("\t%d: x = %f y = %f\n", i, xs[i], ys[i]);
        cities.push_back(new City(xs[i], ys[i], i));

        cities[i]->print();
    }


    Path* path = new Path(cities);



    printf("\n\nCusto do caminho inicial %f\n", path->cost);

    free(xs); free(ys);
    return 0;
}


void distributeCities(float *xs, float *ys){
    //@TODO adicionar codigo MPI para distribuir os valores
    return;
}


