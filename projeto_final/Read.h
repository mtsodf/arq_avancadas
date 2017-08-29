#ifndef Read_H
#define Read_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "City.h"

using namespace std;

void ler_cidades(char* file, int *n, double **xs, double **ys){

    FILE* entrada;

    char buffer1[300], buffer2[300], name[50];
    string buffer;
    entrada = fopen(file, "r");


    fgets(buffer1, 300, entrada);
    buffer.assign(buffer1);
    int find = buffer.find("NODE_COORD_SECTION");
    while(find < 0){

        find = buffer.find("DIMENSION");
        if(find >= 0){
            sscanf(&buffer[0], "%s %d", buffer2, n);
        }


        fgets(buffer1, 300, entrada);
        buffer.assign(buffer1);
        find = buffer.find("NODE_COORD_SECTION");
    }


    *xs = (double*) malloc((*n)*sizeof(double));
    *ys = (double*) malloc((*n)*sizeof(double));


    double x, y;
    int ind;
    for(int i = 0; i < *n; i++)
    {
        fscanf(entrada, "%d %lf %lf", &ind, &x, &y);
        (*xs)[ind-1] = x; (*ys)[ind-1] = y;
    }

    fclose(entrada);
}

vector<City*> createCityVector(char* file){

    int n;
    double *xs, *ys;

    ler_cidades(file, &n, &xs, &ys);

    vector<City*> cities(n);

    for (size_t i = 0; i < n; i++)
    {
        cities[i] = new City(xs[i], ys[i], i);
    }

    return cities;

}

vector<City*> readSolution(char* file, vector<City*> cities, int n){
    vector<City*> cities_sol(n);

    FILE* sol;
    char buffer1[300];
    string buffer;

    sol = fopen(file, "r");

    fgets(buffer1, 300, sol);
    buffer.assign(buffer1);
    int find = buffer.find("TOUR_SECTION");
    while(find < 0){
        fgets(buffer1, 300, sol);
        buffer.assign(buffer1);
        find = buffer.find("TOUR_SECTION");
    }


    int b;
    for(int i = 0; i < n; i++){
        fscanf(sol, "%d", &b);
         cities_sol[i] = cities[b-1];
      }

    fclose(sol);

    return cities_sol;
}


#endif