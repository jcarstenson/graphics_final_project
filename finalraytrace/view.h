#ifndef CS40VIEW_H
#define CS40VIEW_H

#include <string>
#include "common.h"

namespace cs40{

/* a little struct to manage image/eye properties */
typedef struct view_s{
    vec3 eye;           //eye position

    vec3 origin;        //origin of image plane

    vec3 horiz;         //vector from origin bottom left to
                        //bottom right side of image plane

    vec3 vert;          //vector from origin bottom to
                        //top left side of image plane

    vec3 background;    //background rgb color in 0-1 range

    int nrows, ncols;   //for output image

    std::string fname;  //for name of output png file


    // added components
    float pxlHeight, pxlWidth; //the length of the sides of an individual pixel

} View;

} //namespace
#endif
