#ifndef Opt2_H
#define Opt2_H

#include "Path.h"
#include "utils.h"

void findMiniMinjOpenMP(Path* path, int *mini, int *minj, double* minchange);

class Opt2{

public:
    Path  *path;
    Path  *paths[MAX_THREADS];
    Opt2(Path*);
    bool solveOpenMPWhitoutCritical(Path* path, bool log, int *iters, bool logAllPaths);
    bool solveOpenMP(Path* path, bool log, int *iters,  bool logAllPaths);
    bool solveMPI(Path *path, bool log, int* iters, double *times, bool logAllPaths);
    void copyPaths(int qtd);
};

#endif