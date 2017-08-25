#ifndef AnnealingStealing_H
#define AnnealingStealing_H

#include "Path.h"

void sortSwap(int *, int *, int);

float rand_float();

class AnnealingStealing{
public:
    float temperature;
    float alpha;
    float limit;

    Path  *path;
    AnnealingStealing(Path*, float, float, float);

    void solveOpenMp(bool log, int min_iters, int max_iters, int outer_iters, float accepted_cost);
    bool solve(Path*, bool, int, int, int, float, bool*);
    void solve(bool, int, int, int);
    void solve(bool, int, int, int, float);
    void solve(bool, int, int);
    void solve(bool);
    void solve();
    float updateTemperature();
    float trySwap(Path *path, float temperature);
};

#endif