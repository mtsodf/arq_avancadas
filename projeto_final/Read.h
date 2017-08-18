#ifndef Read_H
#define Read_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

void ler_cidades(int *n, float **xs, float **ys){

    FILE* entrada;

    char buffer1[300], buffer2[300], name[50];
    string buffer;
    entrada = fopen("../ALL_tsp/ch4.tsp", "r");

    ifstream tsp_file ("../ALL_tsp/ch4.tsp");

    fgets(buffer1, 300, entrada);

    sscanf(buffer1, "%s %s\n", buffer2, name);

    printf("Nome do caso: %s\n", name);

    fgets(buffer1, 300, entrada);
    fgets(buffer1, 300, entrada);

    cout << buffer1 << "\n";

    fgets(buffer1, 300, entrada);
    sscanf(buffer1, "%s %d", buffer2, n);

    printf("Quantidade de cidades: %d\n", *n);

    *xs = (float*) malloc((*n)*sizeof(float));
    *ys = (float*) malloc((*n)*sizeof(float));

    fgets(buffer1, 300, entrada);
    fgets(buffer1, 300, entrada);
    fgets(buffer1, 300, entrada);
    fgets(buffer1, 300, entrada);

    float x, y;

    for(int i = 0; i < *n; i++)
    {
        fscanf(entrada, "%f %f", &x, &y);
        (*xs)[i] = x; (*ys)[i] = y;
    }

    fclose(entrada);
}

#endif