#include "City.h"
#include "Path.h"
#include "Read.h"
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "AnnealingStealing.h"
#include "rf-time.h"

using namespace std;

int main(int argc,char *argv[]){

            vector<City*> cities;
            vector<City*> cities_sol;
            srand (time(NULL));

            int num_threads = 1;
            if(argc>1){
                num_threads = atoi(argv[1]);
            }

            int caso = 0;

            if(argc>2) caso = atoi(argv[2]);

            omp_set_num_threads(num_threads);

            FILE* results = fopen("resultados.txt", "a+");

            #pragma omp parallel
            {
                #pragma omp single
                printf("Quantidade de threads %d\n", omp_get_num_threads());
            }
            if(caso == 0){
                cities = createCityVector("../ALL_tsp/att48.tsp");
                cities_sol = readSolution("../ALL_tsp/att48.opt.tour", cities, cities.size());
            }


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

            AnnealingStealing* ann = new AnnealingStealing(path, initTemperature, alpha, limit);

            start = get_clock_msec();

            ann->solveOpenMp(false, min_iters, max_iters, outer_iters, accepted_cost);

            time = get_clock_msec() - start;

            printf("Menor custo %f\n", ann->path->cost);

            printf("Tempo para solucao: %lf\n", time);

            printf("Solucao %f\n", path_sol->cost);

            PrintPath("path.txt", ann->path);

            fprintf(results, "%d %d %f %f\n", caso, num_threads, ann->path->cost, time);
            fclose(results);

            return 0;
}


void distributeCities(float *xs, float *ys){
    //@TODO adicionar codigo MPI para distribuir os valores
    return;
}


