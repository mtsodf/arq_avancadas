#include "City.h"
#include "Path.h"
#include "Read.h"
#include "gtest/gtest.h"
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "Opt2.h"
#include "utils.h"
#include <mpi.h>

using namespace std;


int main(int argc, char *argv[]){

    if(argc < 2){
        printf("Necessario passar caminho do arquivo teste\n");
        return -1;
    }


    double start_time, end_time, solveTime[2];
    int mpi_size, rank;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &mpi_size);

    //Sorteio da semente
    srand(time(NULL));
    int seed = rand();
    MPI_Bcast(&seed, 1, MPI_INT, 0, MPI_COMM_WORLD);
    srand(seed);

    printf("Primeiro numero randomico %d (Processo %d)\n", rand(), rank);


    vector<City*> cities;
    vector<City*> cities_sol;
    int iters;


    cities = createCityVector(argv[1]);

    Path* path = new Path(cities);

    path->scramble();


    printf("Sou o processo %d. Cidades %d\n", rank, cities.size());

    Opt2* opt2Solver = new Opt2(path);

    MPI_Barrier(MPI_COMM_WORLD);

    start_time = get_clock_msec();

    opt2Solver->solveMPI(path, false, &iters, solveTime, false);

    end_time = get_clock_msec();

    if(rank == 0){
        Path *path_sol=NULL;
        if(argc > 3){
            cities_sol = readSolution(argv[3], cities, cities.size());
            path_sol = new Path(cities_sol);
            printf("Solucao opt  %f\n", path_sol->cost);
        }

        printf("Solucao calc          : %lf\n", path->cost);
        printf("Tempo para solucao:   : %lf\n", end_time-start_time);
        printf("Tempo de calculo      : %lf\n", solveTime[0]);
        printf("Tempo para sincronizar: %lf\n", solveTime[1]);
        printf("Numero de iteracoes   : %d\n", iters);


        FILE* resultados;

        if(argc > 2){
            resultados = fopen(argv[2], "a+");
        } else{
            resultados = fopen("resultados__.txt", "a+");
        }

        double cost_sol = path_sol==NULL? 0.0 : path_sol->cost;
        fprintf(resultados, "%s; MPI; %d ; %lf; %lf; %lf; %d; %lf; %lf\n", argv[1], mpi_size, end_time-start_time, solveTime[0], solveTime[1], iters, path->cost, cost_sol);
        fclose(resultados);

    }


    MPI_Finalize();

}