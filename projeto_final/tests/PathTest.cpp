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
  ler_cidades("../../ALL_tsp/ch4.tsp", &n, &xs, &ys);

  vector<City*> cities;

  cities.reserve(n);

  for (size_t i = 0; i < n; i++)
  {
    cities[i] = new City(xs[i], ys[i], i);
  }

  EXPECT_NEAR(cities[0]->distance(cities[1]), 1.0, 1e-6);
  EXPECT_NEAR(cities[1]->distance(cities[0]), 1.0, 1e-6);
  EXPECT_NEAR(cities[1]->distance(cities[2]), sqrt(2.0), 1e-6);
  EXPECT_NEAR(cities[2]->distance(cities[3]), 1.0, 1e-6);
  EXPECT_NEAR(cities[3]->distance(cities[0]), sqrt(2.0), 1e-6);

}

TEST(Path, PathSize){
    //Teste do polimorfismo
    EXPECT_EQ(1,1);

    float *xs, *ys;
    int n;
    ler_cidades("../../ALL_tsp/ch4.tsp", &n, &xs, &ys);

    vector<City*> cities;

    cities.reserve(n);

    for (size_t i = 0; i < n; i++)
    {
      cities.push_back(new City(xs[i], ys[i], n));
    }


    printf("Tamanho do caminho %d\n", cities.size());
    Path* path = new Path(cities);


    EXPECT_EQ(path->size, 4);

    EXPECT_NEAR(path->cost, 2 + 2*sqrt(2), 1e-6);


}


TEST(TSP, AnnealingStealing_4){
  //Teste do polimorfismo
  EXPECT_EQ(1,1);

  float *xs, *ys;
  int n;
  ler_cidades("../../ALL_tsp/ch4.tsp", &n, &xs, &ys);

  vector<City*> cities;

  cities.reserve(n);

  for (size_t i = 0; i < n; i++)
  {
    cities.push_back(new City(xs[i], ys[i], n));
  }


  printf("Tamanho do caminho %d\n", cities.size());
  Path* path = new Path(cities);


  EXPECT_EQ(path->size, 4);

  EXPECT_NEAR(path->cost, 2 + 2*sqrt(2), 1e-6);

  AnnealingStealing* ann = new AnnealingStealing(path, 100.0, 0.9);

  ann->solve();

  EXPECT_NEAR(4.0, path->cost, 1e-6);

  free(xs);
  free(ys);


}

TEST(TSP, AnnealingStealing_48){
  //Teste do polimorfismo

  float *xs, *ys;
  int n;

  ler_cidades("../../ALL_tsp/att48.tsp", &n, &xs, &ys);

  vector<City*> cities;

  cities.reserve(n);

  for (size_t i = 0; i < n; i++)
  {
    cities.push_back(new City(xs[i], ys[i], n));
  }


  printf("Tamanho do caminho %d\n", cities.size());
  Path* path = new Path(cities);


  EXPECT_EQ(path->size, 48);

  //EXPECT_NEAR(path->cost, 2 + 2*sqrt(2), 1e-6);

  AnnealingStealing* ann = new AnnealingStealing(path, 100.0, 0.9);


  ann->solve(true);


  free(xs);
  free(ys);



}