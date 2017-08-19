#ifndef AnnealingStealing_H
#define AnnealingStealing_H

#include "Path.h"

class AnnealingStealing{
public:
    float temperature;
    float updateTemperature();
    float trySwap();
    Path  *path;
    AnnealingStealing(Path*, float);
    void solve();
};

#endif