#ifndef CS40LIGHT_H
#define CS40LIGHT_H

#include "common.h"

namespace cs40{

/* a little struct to manage light properties */
typedef struct light_s{
    vec3 position;     //xyz position in world coordinates
    float intensity;   //should be in range 0-1
} Light;

}
#endif
