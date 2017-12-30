#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"
#include "plane.h"
#include "common.h"

namespace cs40 {

class Triangle : public Plane {

protected:
    vec3 m_pts[3];

    bool contains(vec3 p) const;

public:
    Triangle(vec3 p1, vec3 p2, vec3 p3);

    ~Triangle() { /* do nothing */ }

    void boundBox(vec3 box[]) const;
};

}

#endif // TRIANGLE_H
