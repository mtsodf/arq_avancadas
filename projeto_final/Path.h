//#include <list>
#ifndef Path_H
#define Path_H


#include <vector>
#include "City.h"

using namespace std;

class Path{
    public:
        int size;
        vector<City*> *cities;
        float cost;
        Path(vector<City*> *);
        float swap(int, int j);
        float distNext(int);
        float distPrevious(int);
        ~Path();

    private:
        void initCost();

};

#endif