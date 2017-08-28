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


TEST(Opt2, SolveTest){
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
    //srand (time(NULL));
    //path->scramble();



    Opt2* opt2Solve = new Opt2(path);

    opt2Solve->solve(opt2Solve->path, false);

    printf("Solucao Calc %f\n", path->cost);
    printf("Solucao %f\n", path_sol->cost);

    EXPECT_NEAR(path_sol->cost*1.15, path->cost, 1e-5);



    PrintPath("path.txt", path);

}