#include "shape.h"
#include <iostream>
#include <math.h>

using namespace cs40;

float Shape::maxCoord(float x1, float x2, float x3, float x4) const{

    float w1 = fmax(x1,x2);
    float w2 = fmax(x3,x4);

    return fmax(w1, w2);
}

float Shape::minCoord(float x1, float x2, float x3, float x4) const{

    float w1 = fmin(x1,x2);
    float w2 = fmin(x3,x4);

    return fmin(w1,w2);
}
