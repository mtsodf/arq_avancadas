#ifndef Opt2_H
#define Opt2_H

#include "Path.h"

void findMiniMinjOpenMPNew(Path* path, int *mini, int *minj, double* minchange);

class Opt2{

public:
    Path  *path;
    Opt2(Path*);
    bool solveOpenMP(Path* path, bool log, int *iters,  bool logAllPaths);
    bool solveMPI(Path *path, bool log, int* iters, double *times, bool logAllPaths);
};

#endif