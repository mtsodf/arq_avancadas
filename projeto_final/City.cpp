#include <stdio.h>
#include <cmath>
#include "City.h"


City::City(double x, double y, int id){
    this->x=x;
    this->y=y;
    this->id=id;
}

void City::print(){
    printf("City(%f, %f, %d)\n", x, y, id);
}

double City::distance(City* otherCity){
    double dx = x - (otherCity->x);
    double dy = y - (otherCity->y);
    return sqrt( dx*dx + dy*dy );
}