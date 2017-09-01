#include "City.h"
#include "Path.h"
#include "Read.h"
#include "gtest/gtest.h"
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "Opt2.h"
#include "AnnealingStealing.h"
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
    opt2Solver->solve(path, false, &iters, solveTime, false);
    getTimeCounter(0)->endTimer(totalOptSection);


    Path *path_sol = NULL;
    if(argc > 3){
        cities_sol = readSolution(argv[3], cities, cities.size());
        path_sol = new Path(cities_sol);
        printf("Solucao opt           : %lf\n", path_sol->cost);
    }

    printf("Solucao calc          : %lf\n", path->cost);
    printf("Tempo para solucao    : %lf\n", getTimeCounter(0)->getTotalTime(totalOptSection));
    printf("Tempo de calculo      : %lf\n", solveTime[0]);
    printf("Tempo de calculo      : %lf\n", getTimeCounter(0)->getTotalTime(findMinimumSection));
    printf("Tempo para sincronizar: %lf\n", solveTime[1]);
    printf("Tempo para sincronizar: %lf\n", getTimeCounter(0)->getTotalTime(barrierSection));
    printf("Tempo endParallel     : %lf\n", getTimeCounter(0)->getTotalTime(endParallelSection));
    printf("Tempo para swap       : %lf\n", getTimeCounter(0)->getTotalTime(swapSection));
    printf("Numero de iteracoes   : %d\n", iters);

    FILE* resultados;

    if(argc > 2){
        resultados = fopen(argv[2], "a+");
    } else{
        resultados = fopen("resultados.txt", "a+");
    }


    double cost_sol = path_sol==NULL? 0.0 : path_sol->cost;
    fprintf(resultados, "%s; OpenMP; %d ;%lf; %lf; %lf; %d; %lf; %lf\n", argv[1], num_threads, end_time-start_time, solveTime[0], solveTime[1], iters, path->cost, cost_sol);
    fclose(resultados);



}