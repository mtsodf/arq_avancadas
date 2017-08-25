#include "City.h"
#include "Path.h"
#include "Read.h"
#include "gtest/gtest.h"
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "AnnealingStealing.h"

using namespace std;


bool trySwap(Path * path, float temperature){
  int i, j;
  float cost;

  cost = path->cost;

  sortSwap(&i, &j, path->size);

  path->swapTotal(i,j);

  float delta  = path->cost - cost;


  if(delta < 0){
      return true;
  }else{
      if(exp(-delta/temperature) > rand_float()){
          return true;
      } else{
          path->swapTotal(i,j);
          return true;
      }
  }
}

TEST(City, CityDistance){
  float *xs, *ys;
  int n;

  vector<City*> cities = createCityVector("../../ALL_tsp/ch4.tsp");
  EXPECT_EQ(cities.size(), 4);


  EXPECT_NEAR(cities[0]->distance(cities[1]), 1.0, 1e-6);
  EXPECT_NEAR(cities[1]->distance(cities[0]), 1.0, 1e-6);
  EXPECT_NEAR(cities[1]->distance(cities[2]), sqrt(2.0), 1e-6);
  EXPECT_NEAR(cities[2]->distance(cities[3]), 1.0, 1e-6);
  EXPECT_NEAR(cities[3]->distance(cities[0]), sqrt(2.0), 1e-6);

}

TEST(Path, PathSize){
    float *xs, *ys;
    int n;

    vector<City*> cities = createCityVector("../../ALL_tsp/ch4.tsp");
    EXPECT_EQ(cities.size(), 4);

    printf("Tamanho do caminho %d\n", cities.size());
    Path* path = new Path(cities);

    EXPECT_EQ(path->size, 4);
    EXPECT_NEAR(path->cost, 2 + 2*sqrt(2), 1e-6);

    Path* pathCopy = path->copy();
    EXPECT_EQ(pathCopy->size, 4);
    EXPECT_NEAR(pathCopy->cost, 2 + 2*sqrt(2), 1e-6);

    pathCopy->swap(0,1);
    EXPECT_NEAR(pathCopy->cost, 4, 1e-6);
    EXPECT_NEAR(path->cost, 2 + 2*sqrt(2), 1e-6);

}


TEST(TSP, AnnealingStealing_4){
  //Teste do polimorfismo
  EXPECT_EQ(1,1);


  vector<City*> cities = createCityVector("../../ALL_tsp/ch4.tsp");
  EXPECT_EQ(cities.size(), 4);

  printf("Tamanho do caminho %d\n", cities.size());
  Path* path = new Path(cities);


  EXPECT_EQ(path->size, 4);
  EXPECT_NEAR(path->cost, 2 + 2*sqrt(2), 1e-6);

  AnnealingStealing* ann = new AnnealingStealing(path, 100.0, 0.9, 1e-6);

  ann->solve();

  EXPECT_NEAR(4.0, path->cost, 1e-6);

}

TEST(TSP, AnnealingStealing_48){
  ///return;
  vector<City*> cities = createCityVector("../../ALL_tsp/att48.tsp");
  EXPECT_EQ(cities.size(), 48);
  EXPECT_NEAR(cities[0]->x, 6734, 1e-6);
  EXPECT_NEAR(cities[0]->y, 1453, 1e-6);

  EXPECT_NEAR(cities[47]->x, 3023, 1e-6);
  EXPECT_NEAR(cities[47]->y, 1942, 1e-6);

  printf("CHEGOU AQUI!!\n");
  vector<City*> cities_sol = readSolution("../../ALL_tsp/att48.opt.tour", cities, 48);

  Path *path_sol = new Path(cities_sol);

  printf("Tamanho do caminho %d\n", cities.size());
  Path* path = new Path(cities);

  EXPECT_EQ(path->size, 48);

  AnnealingStealing* ann = new AnnealingStealing(path, 1000.0, 0.9999, 1e-10);

  ann->solve(true, 1000000, 100000);
  printf("Solucao Calc %f\n", path->cost);
  printf("Solucao %f\n", path_sol->cost);

  EXPECT_NEAR(path_sol->cost, path->cost, 1e-5);

}

TEST(TSP, AnnealingStealing_48_2){

    vector<City*> cities = createCityVector("../../ALL_tsp/att48.tsp");
    EXPECT_EQ(cities.size(), 48);
    EXPECT_NEAR(cities[0]->x, 6734, 1e-6);
    EXPECT_NEAR(cities[0]->y, 1453, 1e-6);

    EXPECT_NEAR(cities[47]->x, 3023, 1e-6);
    EXPECT_NEAR(cities[47]->y, 1942, 1e-6);

    printf("CHEGOU AQUI!!\n");
    vector<City*> cities_sol = readSolution("../../ALL_tsp/att48.opt.tour", cities, 48);

    Path *path_sol = new Path(cities_sol);

    printf("Tamanho do caminho %d\n", cities.size());
    Path* path = new Path(cities);

    EXPECT_EQ(path->size, 48);

    AnnealingStealing* ann = new AnnealingStealing(path, 100.0, 0.997, 1e-10);

    ann->solve(true, 2000, 100000, 10);
    printf("Solucao Calc %f\n", path->cost);
    printf("Solucao %f\n", path_sol->cost);

  }


TEST(TSP, AnnealingStealing_100){
  //Teste do polimorfismo

  vector<City*> cities = createCityVector("../../ALL_tsp/rd100.tsp");
  EXPECT_EQ(cities.size(), 100);

  EXPECT_NEAR(cities[0]->x, 1.43775e+02, 1e-5);
  EXPECT_NEAR(cities[0]->y, 8.62630e+02, 1e-5);

  EXPECT_NEAR(cities[99]->x, 4.83637e+02, 1e-5);
  EXPECT_NEAR(cities[99]->y, 1.16325e+02, 1e-5);

  vector<City*> cities_sol = readSolution("../../ALL_tsp/rd100.opt.tour", cities, 100);

  Path *path_sol = new Path(cities_sol);

  printf("Tamanho do caminho %d\n", cities.size());
  Path* path = new Path(cities);

  AnnealingStealing* ann = new AnnealingStealing(path, 100.0, 0.997, 1e-10);

  ann->solve(true, 2000, 1e9, 10);
  printf("Solucao Calc %f\n", path->cost);
  printf("Solucao %f\n", path_sol->cost);

}


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

  Path* path = new Path(cities);

  AnnealingStealing* ann = new AnnealingStealing(path, initTemperature, alpha, limit);

  ann->solveOpenMp(false, min_iters, max_iters, outer_iters, accepted_cost);

  printf("Solucao %f\n", path_sol->cost);

}

TEST(TSP, AnnealingStealing_2152){
  //Teste do polimorfismo
  return;
  vector<City*> cities = createCityVector("../../ALL_tsp/u2152.tsp");
  EXPECT_EQ(cities.size(), 2152);

  Path* path = new Path(cities);

  AnnealingStealing* ann = new AnnealingStealing(path, 30.0, 0.999, 1e-10);

  ann->solve(true, 2000, 1e9, 5);
  printf("Solucao Calc %f\n", path->cost);

}


























