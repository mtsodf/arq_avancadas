#include <stdio.h>
#include <cmath>
#include "City.h"


City::City(float x, float y, int id){
    this->x=x;
    this->y=y;
    this->id=id;
}

void City::print(){
    printf("City(%f, %f, %d)\n", x, y, id);
}

float City::distance(City* otherCity){
    float dx = x - (otherCity->x);
    float dy = y - (otherCity->y);
    return sqrt( dx*dx + dy*dy );
}