#ifndef City_H
#define City_H

#include <stdio.h>

class City {
    public:
        City(float, float, int);
        float x, y;
        int id;
        float distance(City*);
        void print();
};

#endif

