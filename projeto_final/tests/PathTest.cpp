#include "City.h"
#include "Path.h"
#include "Read.h"
#include "gtest/gtest.h"
#include <vector>
#include <math.h>
#include "AnnealingStealing.h"

using namespace std;




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
  //Teste do polimorfismo

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

}