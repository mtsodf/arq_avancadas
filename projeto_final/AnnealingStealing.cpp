#include "AnnealingStealing.h"
#include "Path.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <omp.h>

using namespace std;

void PrintPath(char* filename, Path* path){
    FILE* out = fopen(filename, "w");

    for (size_t i = 0; i < path->size; i++)
    {
        fprintf(out, "%d %f %f\n", path->cities[i]->id, path->cities[i]->x, path->cities[i]->y);
    }

    fclose(out);
}

void sortSwap(int *i, int *j, int n){
    *i = rand() % n;
    *j = rand() % n;
}

float rand_float(){
    return (float)rand()/(float)RAND_MAX;
}

AnnealingStealing::AnnealingStealing(Path *path, float temperature, float alpha, float limit){
    this->path = path;
    this->temperature = temperature;
    this->alpha = alpha;
    this->limit = limit;
}

float AnnealingStealing::trySwap(Path* path, float temperature){
    int i, j;
    float cost;

    cost = path->cost;

    sortSwap(&i, &j, path->size);

    path->swapTotal(i,j);

    float delta  = path->cost - cost;


    if(delta < 0){
        return path->cost;
    }else{
        if(exp(-delta/temperature) > rand_float()){
            return path->cost;
        } else{
            path->swapTotal(i,j);
            return path->cost;
        }
    }
}


float AnnealingStealing::updateTemperature(){
    temperature *= alpha;
}

void AnnealingStealing::solve(){
    solve(false);
}

void AnnealingStealing::solve(bool log){
    int iters;
    solve(log, 0, 2e9);
}

void AnnealingStealing::solve(bool log, int min_iters, int max_iters){
    solve(log, min_iters, max_iters, 1);
}

void AnnealingStealing::solve(bool log, int min_iters, int max_iters, int outer_iters){
    solve(log, min_iters, max_iters, outer_iters, 0.0);
}

void AnnealingStealing::solveOpenMp(bool log, int min_iters, int max_iters, int outer_iters, float accepted_cost){
    bool found_solution = false;

    vector<Path*> paths(4);
    int minId;
    float min;
    #pragma omp parallel shared(found_solution, min, minId)
    {

        int id = omp_get_thread_num();
        paths[id] = path->copy();
        paths[id]->scramble();

        while(!found_solution){
            #pragma omp barrier
            solve(paths[id], false, min_iters, max_iters, outer_iters, accepted_cost, &found_solution);
            //printf("Saiu do loop id=%d\n", id);
            if(paths[id]->cost < accepted_cost){
                //printf("ID %d encontrou a solucao. %f\n", id, paths[id]->cost);
                found_solution = true;
            }

            //printf("Cheguei na barreira 1 %d\n", id);
            #pragma omp barrier
            if( id == 0){
                minId=0; min = paths[0]->cost;
                for(int i = 0; i < omp_get_num_threads(); i++){
                    if(paths[i]->cost < min){
                        minId = i;
                        min = paths[i]->cost;
                    }
                }
            }
            //printf("Cheguei na barreira 2 %d\n", id);
            #pragma omp barrier
            if(id != minId){
                paths[id] = paths[minId]->copy();
            }

        }

        if(id == 0){
            path = paths[minId];
        }

        //printf("SAIU DO LOOP id=%d\n", id);
        //#pragma omp barrier

    }

}

void AnnealingStealing::solve(bool log, int min_iters, int max_iters, int outer_iters, float accepted_cost){
    bool found = false;
    solve(path, log, min_iters, max_iters, outer_iters, accepted_cost, &found);
}

bool AnnealingStealing::solve(Path* path, bool log, int min_iters, int max_iters, int outer_iters, float accepted_cost, bool *found){
    FILE* out;
    if(log) out = fopen("out.txt", "w");
    int iters=0;

    float initTemperature = this->temperature;

    if(log) printf("Iniciando iteracao com %f\n", initTemperature);
    int id = omp_get_thread_num();
    for (int i = 0; i < outer_iters && path->cost > accepted_cost && !(*found) ; i++)
    {
        initTemperature = (this->temperature)*pow(0.8,i);
        do{
            //if(iters%100==0) printf("Iteracao %d id=%d\n", iters, id);
            if(log && iters%1 == 0) fprintf(out, "%8d %e %e\n", iters, path->cost, temperature);
            trySwap(path, initTemperature);
            iters++;
            initTemperature = initTemperature * alpha;
        } while((initTemperature > limit || iters < min_iters) && iters < max_iters && path->cost > accepted_cost && !(*found));
    }
    //printf("Custo %f\n", path->cost);
    if(log) printf("FECHANDO UMA ITERACAO ANNELING\n");
    if(log) fclose(out);
    if(log) PrintPath("path.txt", path);

    return path->cost < accepted_cost;
}



