#ifndef CS40SCENE_H
#define CS40SCENE_H

#include "view.h"
#include "light.h"
#include "shape.h"
#include <vector>
#include <octree.h>

namespace cs40{

//A little struct to store everything to raytrace
typedef struct scene_s{
    cs40::View view;
    float ambient;           //global ambient intensity
    std::vector<cs40::Light> lights;    //other positional lights
    std::vector<cs40::Shape*> objects;  //things to raytrace
    int rays; //number of rays for anti-aliasing
    int depth; //depth
    Octree* oct;
} Scene;

}

#endif // SCENE_H
