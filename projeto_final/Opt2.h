#ifndef Opt2_H
#define Opt2_H

#include "Path.h"

void findMiniMinjOpenMP(Path* path, int *mini, int *minj, double* minchange);

class Opt2{

public:
    Path  *path;
    Opt2(Path*);
    bool solve(Path* path, bool log);
    bool solve(Path* path, bool log, int *iters, double *times, bool logAllPaths);
    bool solveOpenMP(Path* path, bool log, int *iters, double *times, bool logAllPaths);
    bool solveMPI(Path *path, bool log, int* iters, double *times, bool logAllPaths);
};

#endif