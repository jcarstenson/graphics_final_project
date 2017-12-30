#include "sphere.h"
#include <iostream>
#include <math.h>

using namespace cs40;

Sphere::Sphere(vec3 c, float r) {
    m_center = c;
    m_radius = r;
}


float Sphere::hitTime(const Ray& r) const {

    vec3 d = r.direction;
    vec3 e = r.origin;
    vec3 c = m_center;

    float A = QVector3D::dotProduct(d,d);

    float B = 2*QVector3D::dotProduct((e-c),d);

    // C = (e-c).(e-c)-r^2
    float C = QVector3D::dotProduct((e-c),(e-c))-(m_radius*m_radius);

    float discrim = B*B - 4*A*C;

    if(discrim < 0.) {
        return -1;
    }

    float discrimRoot = sqrt(discrim);

    float tpos = (-1*B + discrimRoot) / (2*A);

    float tneg = (-1*B - discrimRoot) / (2*A);

    return fmin(tpos, tneg);

}


vec3 Sphere::normal(const vec3 &p) const {
    vec3 normal = p-m_center;
    normal.normalize();

    return normal;
}

void Sphere::boundBox(vec3 box[]) const {
    vec3 bmin = vec3(m_center.x()-m_radius, m_center.y()-m_radius, m_center.z()-m_radius);
    vec3 bmax = vec3(m_center.x()+m_radius, m_center.y()+m_radius, m_center.z()+m_radius);

    box[0] = bmin;
    box[1] = bmax;
}
