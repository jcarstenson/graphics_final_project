#ifndef PLANE_H
#define PLANE_H

#include "shape.h"
#include "common.h"
#include "ray.h"

namespace cs40{

class Plane : public Shape {

protected:

    vec3 m_normal;

    vec3 m_origin;

    /* need to check if a point on the plane actually lies
     * within the shape itself */
    virtual bool contains(vec3 p) const = 0;

    bool leftOf(vec3 p1, vec3 p2, vec3 p3) const;

public:

    ~Plane() { /* do nothing */ }

    /* return a time t for which the ray r = p + v*t would
     * hit the surface of the shape. return t<0 if the ray does
     * not intersect the surface */
    float hitTime(const cs40::Ray& r) const;

    /* return a normal to the surface of the shape at point p,
     * assuming p is on the surface */
    inline vec3 normal(const vec3& p) const {return m_normal;}

    virtual void boundBox(vec3 box[]) const = 0;
};

}

#endif // PLANE_H
