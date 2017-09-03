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

TEST(Path, totalSwap){

  float *xs, *ys;
  int n;

  vector<City*> cities = createCityVector("../../ALL_tsp/ch6.tsp");
  EXPECT_EQ(cities.size(), 6);


  EXPECT_NEAR(cities[0]->distance(cities[1]), 2.0, 1e-6);
  EXPECT_NEAR(cities[1]->distance(cities[0]), 2.0, 1e-6);

  Path * path = new Path(cities);
  EXPECT_EQ(path->size, 6);

  EXPECT_NEAR(path->dist(0,1), 2.0, 1e-6);
  EXPECT_NEAR(path->dist(1,2), 2.0*sqrt(2), 1e-6);
  EXPECT_NEAR(path->dist(2,3), 2.0, 1e-6);
  EXPECT_NEAR(path->dist(3,4), 2.0, 1e-6);
  EXPECT_NEAR(path->dist(4,5), 2.0*sqrt(2), 1e-6);
  EXPECT_NEAR(path->dist(5,0), 2.0, 1e-6);

  EXPECT_NEAR(path->cost, 8 + 4*sqrt(2), 1e-6);

  int mini, minj; double minchange;
  findMiniMinjOpenMP(path, &mini, &minj, &minchange);

  EXPECT_EQ(1, mini);
  EXPECT_EQ(4, minj);

  path->swapTotal(mini+1, minj);

  EXPECT_NEAR(-minchange, 4*(sqrt(2)-1), 1e-6);
  EXPECT_NEAR(path->cost, 8 + 4*sqrt(2) + minchange, 1e-6);
  EXPECT_NEAR(path->cost, 12, 1e-6);

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

    pathCopy->swapTotal(0,3);
    EXPECT_NEAR(pathCopy->cost, 4, 1e-6);
}

