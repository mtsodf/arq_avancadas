#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

using namespace std;

struct cidade{
    int id;
    float x;
    float y;
};

typedef cidade cidade;

cidade** ler_cidades(int  *n){

    cidade** cidades;

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

    cidades = (cidade**) malloc(sizeof(cidade*));

    fgets(buffer1, 300, entrada);
    fgets(buffer1, 300, entrada);
    fgets(buffer1, 300, entrada);
    fgets(buffer1, 300, entrada);

    for(int i = 0; i < *n; i++){
        cidade c;
        c.id = i;
        fscanf(entrada, "%f %f", &c.x, &c.y);

        cidades[i] = &c;

        printf("Cidade %d (%f, %f)\n", c.id, c.x, c.y);
    }

    fclose(entrada);

    return cidades;
}

int main(int argc, char *argv[]){
    omp_set_num_threads(4);
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        printf("Hello World! Eu sou o processo %d\n", id);
        //cout << "Hello World! Eu sou o processo " << id << "\n";
    }

    int n;
    cidade **cidades;

    cidades = ler_cidades(&n);


    return 0;
}