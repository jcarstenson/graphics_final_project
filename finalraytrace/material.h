#ifndef CS40MATERIAL_H
#define CS40MATERIAL_H

#include "common.h"

namespace cs40{

/* a little struct to store material colors */
typedef struct mat_s{
    vec3 ambient; //All in rgb space with 0-1 range
    vec3 diffuse;
    vec3 specular;
    float shiny;
} Material;

} //namespace

#endif
