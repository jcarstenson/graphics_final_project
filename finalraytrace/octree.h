/* Josh Wolper & Jacob Carstenson
 * Octree Header file
 * November 26th, 2014 */

#ifndef OCTREE_H
#define OCTREE_H

#include "shape.h"
#include "ray.h"
#include "common.h"
#include <vector>

namespace cs40{

class Octree {

  private:
    int o_maxData;

    Octree* o_children[8];  //array of child pointers
    Shape** o_shapes;
    int o_numShapes;          //tells how many shapes are held in this octant

    vec3 o_min;
    vec3 o_max;

    inline bool isLeaf() const {return o_children[0] == NULL; }

    // helper function for insert
    std::vector<int> getChildrenInsideBox(const vec3& bmin, const vec3& bmax);

    // helper traversal funciton
    void getSubtreeIntersection(vec3 t0, vec3 t1, const unsigned char a, std::vector<Octree*>& results);

    // helper to getSubtreIntersection
    int firstNode(vec3 t0, vec3 tm);

    // helper to getSubtreeIntersection
    int nextNode(float f1, int n1, float f2, int n2, float f3, int n3);



  public:

    vec3 o_origin;          //origin of node
    float o_halfDim;        //half the dimension (width/height)

    Octree(const vec3& origin, const float halfDim, const int maxData);  //constructor

    ~Octree();

    //int getChildContainingPoint(const vec3& point);     //which child has given point
    
    void insert(Shape* s);    //insert data, will insert a new octree at the node if data already contained at this node when inserting

    // should only be called on the root tree
    std::vector<Octree*> getIntersections(Ray r);

    std::vector<Shape*> getShapes();

};

} //namespace

#endif
