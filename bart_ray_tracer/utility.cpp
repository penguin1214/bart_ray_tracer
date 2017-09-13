#ifndef UTILITY_H
#include "utility.h"

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

float deg2rad(float degree) {
    return degree*M_PI/180;
}

#endif 
