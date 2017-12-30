#ifndef CS40RAY_H
#define CS40RAY_H

#include <string>
#include <iostream>
#include "common.h"

/* A basic Ray class.*/
namespace cs40{

class Ray{

public:
    //go ahead and mess with these directly, they are public
    vec3 origin; //a point
    vec3 direction;  //a vector

    Ray();  /*at origin in -z direction*/
    Ray(const Ray& other); /*copy constructor*/
    Ray(const vec3& origin, const vec3& dir);

    /* convert Ray to string representation */
    std::string str() const;

    //Return point in direction of ray at time t
    inline vec3 operator() (float t) const {return origin+t*direction; }
};

} //namespace

/* allow cout << ray */
std::ostream& operator<<(std::ostream& os, const cs40::Ray& r);

#endif
