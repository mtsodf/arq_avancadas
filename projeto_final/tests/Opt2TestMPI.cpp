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
#include "rf-time.h"

using namespace std;


TEST(Opt2, SolveTest_442){
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