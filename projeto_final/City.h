#ifndef City_H
#define City_H

#include <stdio.h>

class City {
    public:
        City(double, double, int);
        double x, y;
        int id;
        double distance(City*);
        void print();
};

#endif

