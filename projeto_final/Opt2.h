#ifndef Opt2_H
#define Opt2_H

#include "Path.h"

void findMiniMinj(Path* path, int *mini, int *minj, float* minchange);

class Opt2{

public:
    Path  *path;
    Opt2(Path*);
    bool solve(Path* path, bool log);
};

#endif