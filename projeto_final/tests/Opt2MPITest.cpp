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
#include <mpi.h>

using namespace std;


int main(int argc, char *argv[]){
    //return;
    //srand(time(NULL));
    double start_time, end_time;
    int mpi_size, rank;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);

    vector<City*> cities;
    vector<City*> cities_sol;
    cities = createCityVector("../../ALL_tsp/pcb442.tsp");

    Path* path = new Path(cities);

    printf("Sou o processo %d. Cidades %d\n", rank, cities.size());

    Opt2* opt2Solver = new Opt2(path);
    MPI_Barrier(MPI_COMM_WORLD);

    start_time = get_clock_msec();
    opt2Solver->solveMPI(path, false);
    end_time = get_clock_msec();

    if(rank == 0){
        printf("Solucao calc %f\n", path->cost);
        printf("Tempo para solucao: %f", end_time-start_time);
    }



    MPI_Finalize();


    //PrintPath("path.txt", path);

}