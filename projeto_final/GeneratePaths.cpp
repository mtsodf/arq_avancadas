#include "City.h"
#include "Path.h"
#include "Read.h"
#include <stdlib.h>
#include <omp.h>
#include "Opt2.h"
#include "utils.h"

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

    Path *path = new Path(cities);

    Opt2 *opt = new Opt2(path);

    int iters;
    opt->solveOpenMP(path, false, &iters, true);

    printf("Fim do Solver. Resultado %lf\n", path->cost);

    return 0;
}

