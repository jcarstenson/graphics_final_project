#include "plane.h"
#include <iostream>
#include <math.h>

using namespace cs40;


float Plane::hitTime(const Ray &r) const {

    vec3 d = r.direction;
    vec3 e = r.origin;
    vec3 n = m_normal;
    vec3 o = m_origin;

    if(QVector3D::dotProduct(n,d) == 0) {
        return -1;
    }

    float t = (QVector3D::dotProduct(n,(o-e)))/(QVector3D::dotProduct(n,d));

    vec3 p = r(t);

    if(contains(p)) {
        return t;
    } else {
        return -1;
    }
}


bool Plane::leftOf(vec3 p1, vec3 p2, vec3 p3) const {

    vec3 a = p3-p2;
    vec3 b = p1-p2;

    vec3 c = QVector3D::crossProduct(a,b);

    return QVector3D::dotProduct(m_normal,c) > 0;
}
