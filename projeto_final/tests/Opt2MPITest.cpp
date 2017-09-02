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

    //path->scramble();

    initTimers(rank);

    printf("Sou o processo %d. Cidades %d\n", rank, cities.size());

    Opt2* opt2Solver = new Opt2(path);

    MPI_Barrier(MPI_COMM_WORLD);

    getTimeCounter(rank)->startTimer(totalOptSection);
    opt2Solver->solveMPI(path, false, &iters, solveTime, false);
    getTimeCounter(rank)->endTimer(totalOptSection);

    char run_id[200];
    getRunId(run_id);
    MPI_Bcast(run_id, 200, MPI_CHAR, 0, MPI_COMM_WORLD);

    printf("RUN ID %s\n", run_id);


    if(rank == 0){
        Path *path_sol=NULL;
        if(argc > 3){
            cities_sol = readSolution(argv[3], cities, cities.size());
            path_sol = new Path(cities_sol);
            printf("Solucao opt  %f\n", path_sol->cost);
        }

        printf("Solucao calc          : %lf\n", path->cost);
        printf("Tempo para solucao    : %lf\n", getTimeCounter(0)->getTotalTime(totalOptSection));
        printf("Tempo de calculo      : %lf\n", getTimeCounter(0)->getTotalTime(findMinimumSection));
        printf("Tempo para sincronizar: %lf\n", getTimeCounter(0)->getTotalTime(barrierSection));
        printf("Tempo endParallel     : %lf\n", getTimeCounter(0)->getTotalTime(endParallelSection));
        printf("Tempo para swap       : %lf\n", getTimeCounter(0)->getTotalTime(swapSection));
        printf("Tempo para critical   : %lf\n", getTimeCounter(0)->getTotalTime(criticalSection));
        printf("Numero de iteracoes   : %d\n", iters);


        FILE* resultados;

        if(argc > 2){
            resultados = fopen(argv[2], "a+");
        } else{
            resultados = fopen("resultados__.txt", "a+");
        }


        double cost_sol = path_sol==NULL? 0.0 : path_sol->cost;
        fprintf(resultados, "%s; %s; MPI; %d; ", run_id, argv[1], mpi_size);
        fprintf(resultados, "%lf; ", getTimeCounter(0)->getTotalTime(totalOptSection));
        fprintf(resultados, "%lf; ", getTimeCounter(0)->getTotalTime(findMinimumSection));
        fprintf(resultados, "%lf; ", getTimeCounter(0)->getTotalTime(barrierSection));
        fprintf(resultados, "%lf; ", getTimeCounter(0)->getTotalTime(swapSection));
        fprintf(resultados, "%lf; ", path->cost);
        fprintf(resultados, "%lf;\n", cost_sol);

        fclose(resultados);

    }

    char filename[50];
    sprintf(filename, "%s.json", run_id);


    if(rank == 0){
        FILE* resultados = fopen(filename, "a+");
        fprintf(resultados, "{\n");
        fprintf(resultados, "\"id\": %s,\n", run_id);
        fprintf(resultados, "\"file\": \"%s\",\n", argv[1]);
        fprintf(resultados, "\"size\": %d,\n", mpi_size);
        fprintf(resultados, "\"run\": [\n");
        fclose(resultados);
    }

    for (size_t i = 0; i < mpi_size; i++)
    {
        if(i == rank){
            FILE* resultados = fopen(filename, "a+");

            fprintf(resultados, "{\n");
            fprintf(resultados, "\t\"rank\": %d,\n", i);
            fprintf(resultados, "\t\"solTime\": %lf,\n",     getTimeCounter(rank)->getTotalTime(totalOptSection));
            fprintf(resultados, "\t\"parallelTime\": %lf,\n",getTimeCounter(rank)->getTotalTime(findMinimumSection));
            fprintf(resultados, "\t\"serialTime\": %lf,\n",  getTimeCounter(rank)->getTotalTime(barrierSection));
            fprintf(resultados, "\t\"swapTime\": %lf,\n",    getTimeCounter(rank)->getTotalTime(swapSection));
            fprintf(resultados, "\t\"criticalTime\": %lf\n", getTimeCounter(rank)->getTotalTime(criticalSection));
            if(rank != mpi_size - 1){
                fprintf(resultados, "},\n");
            } else{
                fprintf(resultados, "}\n");
                fprintf(resultados, "]\n");
                fprintf(resultados, "}\n");
            }



            fclose(resultados);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }



    MPI_Finalize();

}