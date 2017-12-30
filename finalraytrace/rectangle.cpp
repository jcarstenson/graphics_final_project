#include "rectangle.h"
#include <iostream>
#include <math.h>

using namespace cs40;

Rectangle::Rectangle(vec3 ll, vec3 lr, vec3 ur) {

    m_pts[0] = ll;
    m_pts[1] = lr;
    m_pts[2] = ur;

    vec3 h = ur - lr;

    m_pts[3] = ll + h;

    // find the midpoint between ll and ur
    m_origin = (ll+ur)/2.0;

    // find the cross product between (lr-ll) and (ur-lr)
    // to get the normal vector
    m_normal = QVector3D::crossProduct((lr-ll), (ur-lr));
    m_normal.normalize();
}


bool Rectangle::contains(vec3 p) const{

    //check left of for all point couples except last to first
    for(int i = 0; i < 3; i++){
        if(!leftOf(p, m_pts[i], m_pts[i+1])){
            return false;
        }
    }

    //check left of for last to first
    if (!leftOf(p, m_pts[3], m_pts[0])){
        return false;
    }

    return true;
}

void Rectangle::boundBox(vec3 box[]) const{

    vec3 ll = m_pts[0];
    vec3 lr = m_pts[1];
    vec3 ur = m_pts[2];
    vec3 ul = m_pts[3];

    float xmin, ymin, zmin;
    float xmax, ymax, zmax;

    xmax = maxCoord(ll.x(),lr.x(),ur.x(),ul.x());
    ymax = maxCoord(ll.y(),lr.y(),ur.y(),ul.y());
    zmax = maxCoord(ll.z(),lr.z(),ur.z(),ul.z());
    xmin = maxCoord(ll.x(),lr.x(),ur.x(),ul.x());
    ymin = maxCoord(ll.y(),lr.y(),ur.y(),ul.y());
    zmin = maxCoord(ll.z(),lr.z(),ur.z(),ul.z());

    vec3 b1 = vec3(xmin, ymin, zmin);
    vec3 b2 = vec3(xmax, ymax, zmax);

    box[0] = b1;
    box[1] = b2;

}


