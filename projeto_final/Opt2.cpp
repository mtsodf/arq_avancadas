#include "Opt2.h"
#include <stdlib.h>

void findMiniMinj(Path* path, int *mini, int *minj, float* minchange){
    int inext, jnext;
    float change;

    *minchange = 0.0;

    for(int i = 0; i < path->size-2;i++){
        inext = (i+1)%(path->size);
        for(int j = i+2; j < path->size; j++){
            jnext = (j+1)%(path->size);
            change = path->dist(i,j) + path->dist(inext, jnext) - path->dist(i,inext) - path->dist(j, jnext);
            if(*minchange > change){
                *minchange = change;
                *mini = i;
                *minj = j;
            }
        }
    }
}


Opt2::Opt2(Path* path){
    this->path = path;
}

bool Opt2::solve(Path* path, bool log){

    float minchange = 0.0;
    int mini, minj;
    int iters = 0;

    do{
        findMiniMinj(path, &mini, &minj, &minchange);
        if(minchange < 0.0)  path->swapTotal(mini+1, minj);
        iters++;
    } while(minchange < 0.0);


}