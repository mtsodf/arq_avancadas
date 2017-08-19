#ifndef Read_H
#define Read_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

void ler_cidades(char* file, int *n, float **xs, float **ys){

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


    *xs = (float*) malloc((*n)*sizeof(float));
    *ys = (float*) malloc((*n)*sizeof(float));


    float x, y;

    for(int i = 0; i < *n; i++)
    {
        fscanf(entrada, "%f %f", &x, &y);
        (*xs)[i] = x; (*ys)[i] = y;
    }

    fclose(entrada);
}

#endif