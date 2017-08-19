#ifndef AnnealingStealing_H
#define AnnealingStealing_H

#include "Path.h"

class AnnealingStealing{
public:
    float temperature;
    float alpha;
    float updateTemperature();
    float trySwap();
    Path  *path;
    AnnealingStealing(Path*, float, float);
    void solve(bool);
    void solve();
};

#endif