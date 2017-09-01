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

using namespace std;


TEST(Opt2, SolveTest_48){
    return;
    vector<City*> cities;
    vector<City*> cities_sol;
    double start_time, end_time;
    int num_threads;
    cities = createCityVector("../../ALL_tsp/att48.tsp");
    EXPECT_EQ(cities.size(), 48);
    EXPECT_NEAR(cities[0]->x, 6734, 1e-6);
    EXPECT_NEAR(cities[0]->y, 1453, 1e-6);

    EXPECT_NEAR(cities[47]->x, 3023, 1e-6);
    EXPECT_NEAR(cities[47]->y, 1942, 1e-6);

    #pragma omp parallel
    #pragma omp single
    {
        num_threads = omp_get_num_threads();
        printf("Rodando com %d threads\n", num_threads);

    }


    printf("rand 1 %d\n", rand());
    //printf("rand 2 %d\n", rand());
    //printf("rand 3 %d\n", rand());

    cities_sol = readSolution("../../ALL_tsp/att48.opt.tour", cities, cities.size());


    Path *path_sol = new Path(cities_sol);

    printf("Tamanho do caminho %d\n", cities.size());

    Path* path = new Path(cities);

    path->scramble();

    Opt2* opt2Solve = new Opt2(path);

    start_time = get_clock_msec();
    opt2Solve->solve(opt2Solve->path, false);
    end_time = get_clock_msec();

    printf("Tempo para conseguir solucao %f\n", end_time-start_time);
    printf("Solucao Calc %f\n", path->cost);
    printf("Solucao      %f\n", path_sol->cost);

    EXPECT_GT(path_sol->cost*1.10, path->cost);

    PrintPath("path.txt", path);

}

TEST(Opt2, SolveTest_200){
    srand(time(NULL));
    vector<City*> cities;
    vector<City*> cities_sol;
    cities = createCityVector("../../ALL_tsp/att48.tsp");
    EXPECT_EQ(cities.size(), 48);
    EXPECT_NEAR(cities[0]->x, 6734, 1e-6);
    EXPECT_NEAR(cities[0]->y, 1453, 1e-6);

    EXPECT_NEAR(cities[47]->x, 3023, 1e-6);
    EXPECT_NEAR(cities[47]->y, 1942, 1e-6);

    cities_sol = readSolution("../../ALL_tsp/att48.opt.tour", cities, cities.size());

    cities = createCityVector("../../ALL_tsp/rd100.tsp");
    EXPECT_EQ(cities.size(), 100);

    EXPECT_NEAR(cities[0]->x, 1.43775e+02, 1e-5);
    EXPECT_NEAR(cities[0]->y, 8.62630e+02, 1e-5);

    EXPECT_NEAR(cities[99]->x, 4.83637e+02, 1e-5);
    EXPECT_NEAR(cities[99]->y, 1.16325e+02, 1e-5);

    cities_sol = readSolution("../../ALL_tsp/rd100.opt.tour", cities, 100);

    Path *path_sol = new Path(cities_sol);

    printf("Tamanho do caminho %d\n", cities.size());

    Path* path = new Path(cities);

    path->scramble();

    Opt2* opt2Solve = new Opt2(path);

    opt2Solve->solve(opt2Solve->path, false);

    printf("Solucao Calc %f\n", path->cost);
    printf("Solucao      %f\n", path_sol->cost);

    EXPECT_GT(path_sol->cost*1.10, path->cost);

    PrintPath("path.txt", path);

}


TEST(Opt2, SolveTest_1379){
    return;
    //srand(time(NULL));
    vector<City*> cities;
    double start_time, end_time;
    int num_threads;

    #pragma omp parallel
    #pragma omp single
    {
        num_threads = omp_get_num_threads();
        printf("Rodando com %d threads\n", num_threads);

    }

    cities = createCityVector("../../ALL_tsp/nrw1379.tsp");
    EXPECT_EQ(cities.size(), 1379);

    EXPECT_NEAR(cities[0]->x, 2918, 1e-6);
    EXPECT_NEAR(cities[0]->y, 6528, 1e-6);

    EXPECT_NEAR(cities[1]->x, 2925, 1e-6);
    EXPECT_NEAR(cities[1]->y, 6597, 1e-6);


    Path* path = new Path(cities);

    path->scramble();

    Opt2* opt2Solve = new Opt2(path);

    start_time = get_clock_msec();
    opt2Solve->solve(opt2Solve->path, false);
    end_time = get_clock_msec();
    printf("Tempo para conseguir solucao %f\n", end_time-start_time);

    printf("Solucao Calc %f\n", path->cost);

    PrintPath("path.txt", path);

}

TEST(Opt2, SolveTest_442){
    //srand(time(NULL));
    vector<City*> cities;
    double start_time, end_time;
    int num_threads;

    #pragma omp parallel
    #pragma omp single
    {
        num_threads = omp_get_num_threads();
        printf("Rodando com %d threads\n", num_threads);

    }

    cities = createCityVector("../../ALL_tsp/pcb442.tsp");

    EXPECT_EQ(cities.size(), 442);

    EXPECT_NEAR(cities[0]->x, 2.00000e+02, 1e-6);
    EXPECT_NEAR(cities[0]->y, 4.00000e+02, 1e-6);

    EXPECT_NEAR(cities[441]->x, 0.00000e+00, 1e-6);
    EXPECT_NEAR(cities[441]->y, 0.00000e+00, 1e-6);
    Path* path = new Path(cities);

    vector<City*> cities_sol;
    cities_sol = readSolution("../../ALL_tsp/pcb442.opt.tour", cities, cities.size());
    Path* path_sol = new Path(cities_sol);

    path->scramble();

    Opt2* opt2Solve = new Opt2(path);

    start_time = get_clock_msec();
    opt2Solve->solve(opt2Solve->path, false);
    end_time = get_clock_msec();
    printf("Tempo para conseguir solucao %f\n", end_time-start_time);


    printf("Solucao Calc %f\n", path->cost);
    printf("Solucao      %f\n", path_sol->cost);

    EXPECT_GT(path_sol->cost*1.15, path->cost);


    PrintPath("path.txt", path);

}

