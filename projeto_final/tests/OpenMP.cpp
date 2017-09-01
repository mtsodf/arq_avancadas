#include "City.h"
#include "Path.h"
#include "Read.h"
#include "gtest/gtest.h"
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "AnnealingStealing.h"
#include "utils.h"


TEST(TSP, AnnealingStealing_OPENMP){

        vector<City*> cities = createCityVector("../../ALL_tsp/att48.tsp");

        vector<City*> cities_sol = readSolution("../../ALL_tsp/att48.opt.tour", cities, cities.size());

        Path *path_sol = new Path(cities_sol);

        const int outer_iters = 1;
        const int accepted_cost = 34000;
        const int max_iters = 20000;
        const int min_iters = 0;
        float initTemperature = 100.0;
        float alpha = 0.997;
        float limit = 1e-10;

        double start, time;

        Path* path = new Path(cities);

        printf("Rodando com %d threads.\n", omp_get_num_threads());

        AnnealingStealing* ann = new AnnealingStealing(path, initTemperature, alpha, limit);

        start = get_clock_msec();

        ann->solveOpenMp(false, min_iters, max_iters, outer_iters, accepted_cost);

        time = get_clock_msec() - start;

        printf("Menor custo %d %f\n", 0, ann->path->cost);

        printf("Tempo para solucao: %lf\n", time);

        printf("Solucao %f\n", path_sol->cost);

        PrintPath("path.txt", ann->path);

}