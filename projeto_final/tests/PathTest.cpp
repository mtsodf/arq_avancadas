#include "City.h"
#include "Path.h"
#include "Read.h"
#include "gtest/gtest.h"
#include <vector>
#include <math.h>
#include <omp.h>
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
  //Teste do polimorfismo
  return;
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

TEST(TSP, AnnealingStealing_48_2){
    //Teste do polimorfismo
    return ;
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
  return;
  vector<City*> cities = createCityVector("../../ALL_tsp/rd100.tsp");
  EXPECT_EQ(cities.size(), 100);

  EXPECT_NEAR(cities[0]->x, 1.43775e+02, 1e-5);
  EXPECT_NEAR(cities[0]->y, 8.62630e+02, 1e-5);

  EXPECT_NEAR(cities[99]->x, 4.83637e+02, 1e-5);
  EXPECT_NEAR(cities[99]->y, 1.16325e+02, 1e-5);

  printf("CHEGOU AQUI!!\n");
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
  //return;
  vector<City*> cities = createCityVector("../../ALL_tsp/rd100.tsp");
  EXPECT_EQ(cities.size(), 100);

  EXPECT_NEAR(cities[0]->x, 1.43775e+02, 1e-5);
  EXPECT_NEAR(cities[0]->y, 8.62630e+02, 1e-5);

  EXPECT_NEAR(cities[99]->x, 4.83637e+02, 1e-5);
  EXPECT_NEAR(cities[99]->y, 1.16325e+02, 1e-5);

  vector<City*> cities_sol = readSolution("../../ALL_tsp/rd100.opt.tour", cities, 100);


  Path *path_sol = new Path(cities_sol);

  vector<Path*> paths(4);

  printf("Tamanho do caminho %d\n", cities.size());
  #pragma omp parallel
  {
      int id = omp_get_thread_num();
      paths[id] = new Path(cities);
      paths[id]->scramble();

      for(int i = 0; i < omp_get_num_threads(); i++){
        if(i == id) printf("Meu id eh %d. O custo do caminho é %f\n", id, paths[id]->cost);
        #pragma omp barrier
      }

      #pragma omp barrier

      for(int cont = 0; cont < 100; cont ++){

        if(id == 0) printf("Inicio da iteracao %d\n", cont);
        for(int i = 0; i < omp_get_num_threads(); i++){
            if(i == id) printf("Meu id eh %d. O custo do caminho é %f\n", id, paths[id]->cost);
            #pragma omp barrier
        }

        AnnealingStealing* ann = new AnnealingStealing(paths[id], 100.0*pow(0.8, cont), 0.997, 1e-10);
        ann->solve(true, 500, 20000, 1);

        int minId=0; float min = paths[0]->cost;
        if(id == 0) printf("Fim da iteracao %d\n", cont);
        #pragma omp barrier
        for(int i = 0; i < omp_get_num_threads(); i++){
            if(i == id) printf("Meu id eh %d. O custo do caminho é %f\n", id, paths[id]->cost);

            if(paths[i]->cost < min){
                minId = i;
                min = paths[id]->cost;
            }
            #pragma omp barrier
        }
        #pragma omp barrier
        if(id != minId){
            paths[id] = paths[minId]->copy();
        }
        #pragma omp barrier

  }

}


  printf("Solucao Calc %f\n", paths[0]->cost);
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

TEST(TSP, AnnealingStealingParallel_48){


  vector<City*> cities = createCityVector("../../ALL_tsp/rd100.tsp");
  EXPECT_EQ(cities.size(), 100);

  EXPECT_NEAR(cities[0]->x, 1.43775e+02, 1e-5);
  EXPECT_NEAR(cities[0]->y, 8.62630e+02, 1e-5);

  EXPECT_NEAR(cities[99]->x, 4.83637e+02, 1e-5);
  EXPECT_NEAR(cities[99]->y, 1.16325e+02, 1e-5);

  printf("CHEGOU AQUI!!\n");
  vector<City*> cities_sol = readSolution("../../ALL_tsp/rd100.opt.tour", cities, 100);

  Path *path_sol = new Path(cities_sol);

  printf("Tamanho do caminho %d\n", cities.size());
  Path* path = new Path(cities);

  AnnealingStealing* ann = new AnnealingStealing(path, 5000.0, 0.997, 1e-10);

  ann->solveOpenMp(true, 2000, 1e9, 3);

  printf("Solucao Calc %f\n", path->cost);
  printf("Solucao %f\n", path_sol->cost);

}

























