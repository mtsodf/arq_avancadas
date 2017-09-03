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
    srand(time(NULL));

    if(argc < 2){
        printf("Necessario passar caminho do arquivo teste\n");
        return -1;
    }

    double start_time, end_time, solveTime[4];
    vector<City*> cities;
    vector<City*> cities_sol;
    int iters;
    int num_threads;



    #pragma omp parallel
    num_threads = omp_get_num_threads();


    for (size_t i = 0; i < num_threads; i++)
    {
        initTimers(i);
    }

    printf("Rodando com %d threads\n", num_threads);

    cities = createCityVector(argv[1]);

    Path* path = new Path(cities);
    path->scramble();

    Opt2* opt2Solver = new Opt2(path);

    getTimeCounter(0)->startTimer(totalOptSection);
    opt2Solver->solveOpenMP(path, false, &iters, false);
    getTimeCounter(0)->endTimer(totalOptSection);

    char run_id[200];
    getRunId(run_id);

    Path *path_sol = NULL;
    if(argc > 3){
        cities_sol = readSolution(argv[3], cities, cities.size());
        path_sol = new Path(cities_sol);
        printf("Solucao opt           : %lf\n", path_sol->cost);
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
        resultados = fopen("resultados.txt", "a+");
    }


    double cost_sol = path_sol==NULL? 0.0 : path_sol->cost;
    fprintf(resultados, "%s; %s; OpenMP; %d; ", run_id, argv[1], num_threads);
    fprintf(resultados, "%lf; ", getTimeCounter(0)->getTotalTime(totalOptSection));
    fprintf(resultados, "%lf; ", getTimeCounter(0)->getTotalTime(findMinimumSection));
    fprintf(resultados, "%lf; ", getTimeCounter(0)->getTotalTime(barrierSection));
    fprintf(resultados, "%lf; ", getTimeCounter(0)->getTotalTime(swapSection));
    fprintf(resultados, "%d; ", iters);
    fprintf(resultados, "%lf; ", path->cost);
    fprintf(resultados, "%lf;\n", cost_sol);

    fclose(resultados);

    char filename[50];

    sprintf(filename, "%s.json", run_id);
    resultados = fopen(filename, "w");

    fprintf(resultados, "{\n");
    fprintf(resultados, "\"id\": %s,\n", run_id);
    fprintf(resultados, "\"file\": \"%s\",\n", argv[1]);
    fprintf(resultados, "\"type\": \"OpenMP\",\n");
    fprintf(resultados, "\"size\": %d,\n", num_threads);
    fprintf(resultados, "\"run\": [\n");

    for (size_t i = 0; i < num_threads; i++)
    {
        fprintf(resultados, "{\n");
        fprintf(resultados, "\t\"rank\": %d,\n", i);
        fprintf(resultados, "\t\"solTime\": %lf,\n",     getTimeCounter(i)->getTotalTime(totalOptSection));
        fprintf(resultados, "\t\"parallelTime\": %lf,\n",getTimeCounter(i)->getTotalTime(findMinimumSection));
        fprintf(resultados, "\t\"serialTime\": %lf,\n",  getTimeCounter(i)->getTotalTime(barrierSection));
        fprintf(resultados, "\t\"swapTime\": %lf,\n",    getTimeCounter(i)->getTotalTime(swapSection));
        fprintf(resultados, "\t\"criticalTime\": %lf\n", getTimeCounter(i)->getTotalTime(criticalSection));
        if(i != num_threads - 1){
            fprintf(resultados, "},\n");
        } else{
            fprintf(resultados, "}\n");
        }
    }
    fprintf(resultados, "]\n");
    fprintf(resultados, "}\n");
    fclose(resultados);



}