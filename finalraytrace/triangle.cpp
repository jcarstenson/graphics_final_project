#include "triangle.h"
#include <iostream>
#include <math.h>

using namespace cs40;

Triangle::Triangle(vec3 p1, vec3 p2, vec3 p3) {

    m_pts[0] = p1;
    m_pts[1] = p2;
    m_pts[2] = p3;

    // find the centroid

    m_origin = (p1+p2+p3)/3.0;

    // find the cross product between (p2-p1) and (ur-p2)
    // to get the normal vector
    m_normal = QVector3D::crossProduct((p1-p2),(p2-p3));
    m_normal.normalize();
}


bool Triangle::contains(vec3 p) const{

    //check left of for ap1 point couples except last to first
    for(int i = 0; i < 2; i++){
        if(!leftOf(p, m_pts[i], m_pts[i+1])){
            return false;
        }
    }

    //check left of for last to first
    if (!leftOf(p, m_pts[2], m_pts[0])){
        return false;
    }

    return true;
}

void Triangle::boundBox(vec3 box[]) const{

    vec3 p1 = m_pts[0];
    vec3 p2 = m_pts[1];
    vec3 p3 = m_pts[2];

    float xmin, ymin, zmin;
    float xmax, ymax, zmax;

    xmax = maxCoord(p1.x(),p2.x(),p3.x(),p3.x());
    ymax = maxCoord(p1.y(),p2.y(),p3.y(),p3.y());
    zmax = maxCoord(p1.z(),p2.z(),p3.z(),p3.z());

    xmin = minCoord(p1.x(),p2.x(),p3.x(),p3.x());
    ymin = minCoord(p1.y(),p2.y(),p3.y(),p3.y());
    zmin = minCoord(p1.z(),p2.z(),p3.z(),p3.z());

    vec3 b1 = vec3(xmin, ymin, zmin);
    vec3 b2 = vec3(xmax, ymax, zmax);

    box[0] = b1;
    box[1] = b2;

}

