#ifndef CS40SPHERE_H
#define CS40SPHERE_H

#include "shape.h"
#include "common.h"
#include "ray.h"

namespace cs40{

class Sphere : public Shape { 

protected:
    vec3 m_center;
    float m_radius;

public:
    Sphere(vec3 c, float r);

    ~Sphere() { /*do nothing*/ };

    float hitTime(const cs40::Ray& r) const;
    vec3 normal(const vec3& p) const;
    void boundBox(vec3 box[]) const;
};

}

#endif
