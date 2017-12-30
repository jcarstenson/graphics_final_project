#ifndef CS40SHAPE_H
#define CS40SHAPE_H

#include "common.h"
#include "ray.h"
#include "material.h"

namespace cs40{

class Shape { 
public:
    cs40::Material material; //color information

    virtual ~Shape() { /*do nothing*/ };
    
    /* return a time t for which the ray r = p + v*t would
     * hit the surface of the shape. return t<0 if the ray does
     * not intersect the surface */
    virtual float hitTime(const cs40::Ray& r) const = 0;

    /* return a normal to the surface of the shape at point p,
     * assuming p is on the surface */
    virtual vec3 normal(const vec3& p) const = 0;

    /* return min and max bounding box corners
     * assuming min is lower left and max is upper right*/
    virtual void boundBox(vec3 box[]) const = 0;

    float maxCoord(float x1, float x2, float x3, float x4) const;
    float minCoord(float x1, float x2, float x3, float x4) const;

};

} //namespace
#endif
