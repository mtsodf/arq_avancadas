#ifndef AnnealingStealing_H
#define AnnealingStealing_H

#include "Path.h"

class AnnealingStealing{
public:
    float temperature;
    float alpha;
    float limit;
    float updateTemperature();
    float trySwap();
    Path  *path;
    AnnealingStealing(Path*, float, float, float);
    void solve(bool, int, int);
    void solve(bool);
    void solve();
};

#endif