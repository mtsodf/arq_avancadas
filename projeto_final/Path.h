//#include <list>
#ifndef Path_H
#define Path_H


#include <vector>
#include "City.h"

using namespace std;

class Path{
    public:
        int size;
        vector<City*> cities;
        double cost;
        Path(vector<City*>);
        double swap(int, int j);
        double swapTotal(int, int j);
        double dist(int i, int j);
        double distNext(int);
        double distPrevious(int);
        void scramble();
        Path * copy();
        ~Path();

    private:
        void initCost();

};

#endif