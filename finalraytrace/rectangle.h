#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include "plane.h"
#include "common.h"

namespace cs40 {

class Rectangle : public Plane {

protected:
    vec3 m_pts[4];

    bool contains(vec3 p) const;


public:
    Rectangle(vec3 ll, vec3 lr, vec3 ur);

    ~Rectangle() { /* do nothing */ }

    void boundBox(vec3 box[]) const;
};

}




#endif // RECTANGLE_H
