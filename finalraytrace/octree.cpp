/* Josh Wolper & Jacob Carstenson
 * Octree Implementation file
 * November 26th, 2014 */

#include "octree.h"
#include <math.h>

using namespace cs40;

//Constructor
Octree::Octree(const vec3& origin, const float halfDim, const int maxData){
    o_origin = origin;
    o_halfDim = halfDim;
    o_numShapes = 0;
    o_maxData = maxData;
    for(int i=0;i<8;i++){
        o_children[i] = NULL;
    }

    o_shapes = new Shape*[o_maxData];

    for(int i=0; i<o_maxData; i++) {
        o_shapes[i] = NULL;
    }

    vec3 o = origin;

    o_min = vec3(o.x()-halfDim, o.y()-halfDim, o.x()-halfDim);
    o_max = vec3(o.x()+halfDim, o.y()+halfDim, o.x()+halfDim);
}

//destructor
Octree::~Octree() {
    for(int i=0; i<8; i++) {
        delete o_shapes[i];
        delete o_children[i];
    }
}

/* Wrote function ourselves but followed someone online's definition of the children.
 * We defined them like so for ease
 *
   Children follow a predictable pattern to make accesses simple.
   Here, - means less than 'origin' in that dimension, + means greater than.
   child: 0 1 2 3 4 5 6 7
       x: - - - - + + + +
       y: - - + + - - + +
       z: - + - + - + - +

int Octree::getChildContainingPoint(const vec3& point){
    //POSITIVE X
    if(point.x() > o_origin.x()){
        //POSITIVE Y
        if(point.y() > o_origin.y()){
            //POSITIVE Z
            if(point.z() > o_origin.z()){
                return 7; //+x +y +z
            }
            //NEGATIVE Z
            else{
                return 6; // +x +y -z
            }
        }
        //NEGATIVE Y
        else{
            //POS Z
            if(point.z() > o_origin.z()){
                return 5; //+x -y +z
            }
            //NEG Z
            else{
                return 4; //+x -y -z
            }
        }
    }
    //NEGATIVE X
    else{
        //POS Y
        if(point.y() > o_origin.y()){
            //POS Z
            if(point.z() > o_origin.z()){
                return 3; //-x +y +z
            }
            //NEG Z
            else{
                return 2; // -x +y -z
            }
        }
        //NEG Y
        else{
            //POS Z
            if(point.z() > o_origin.z()){
                return 1; //-x -y +z
            }
            //NEG Z
            else{
                return 0; //-x -y -z
            }
        }
    }
}
*/

//insert data
void Octree::insert(Shape* s){
    //case1: this is a leaf - then we are done traversing
    if(isLeaf()) {
        // and there is room for the shape
        if(o_numShapes < o_maxData) {
            o_shapes[o_numShapes] = s;
            o_numShapes++;

        // we are a leaf but we are full of shapes so its time to split
        // into 8 more children
        } else {

            //first take out our old shapes to re-insert later
            int shapeNum = o_maxData + 1;
            Shape* allShapes[shapeNum];
            for(int i=0; i<o_maxData; i++) {
                allShapes[i] = o_shapes[i];
                o_shapes[i] = NULL;
            }
            allShapes[shapeNum-1] = s; // add s to this array

            //Split the current node and create new empty trees for each child
            for(int i=0; i<8; i++) {
                vec3 newOrigin = o_origin;

                // chekcing the x bit
                if(i & 4) {
                    newOrigin.setX(newOrigin.x()+0.5*o_halfDim);
                } else {
                    newOrigin.setX(newOrigin.x()-0.5*o_halfDim);
                }

                //checking the y bit
                if(i & 2) {
                    newOrigin.setY(newOrigin.y()+0.5*o_halfDim);
                } else {
                    newOrigin.setY(newOrigin.y()-0.5*o_halfDim);
                }

                //checking the z bit
                if(i & 1) {
                    newOrigin.setZ(newOrigin.z()+0.5*o_halfDim);
                } else {
                    newOrigin.setZ(newOrigin.z()-0.5*o_halfDim);
                }


                o_children[i] = new Octree(newOrigin, 0.5*o_halfDim, o_maxData);
            }

            // re-insert old shapes and new shape
            for(int i=0; i<shapeNum; i++) {
                //find the shape's bounding box
                vec3 box[2];
                allShapes[i]->boundBox(box);

                //find the children contained in this box
                std::vector<int> children = getChildrenInsideBox(box[0], box[1]);

                //Debugging code:
                if(children.size() == 0) {
                  std::cout << "no children for this shape :( it disappeared" << std::endl;
                }

                //insert this shape into each of these children
                while(!children.empty()) {
                    int c = children.back();
                    o_children[c]->insert(allShapes[i]);
                    children.pop_back();
                }    
            }
        }

    // were in an interor none. insert recursively
    } else {
        vec3 box[2];
        s->boundBox(box);
        std::vector<int> children = getChildrenInsideBox(box[0], box[1]);

        // Debugging code:
        if(children.size() == 0) {
            std::cout << "no children for this shape (in interior node). its gone";
            std::cout << std::endl;
        }

        while(!children.empty()) {
            int c = children.back();
            o_children[c]->insert(s);
            children.pop_back();
        }
    }
}

// this function will return all of the children
// that are contained within a bounding box
std::vector<int> Octree::getChildrenInsideBox(const vec3& bmin, const vec3& bmax) {

    std::vector<int> results;
    
    for(int i=0; i<8; i++) {

        vec3 cmax = o_children[i]->o_max;
        vec3 cmin = o_children[i]->o_min;

        //check to see if the query box is outside the child's box
        
        //this is the distance between the origins of the boxes
        vec3 dist = 0.5*(bmax + bmin - cmax - cmin);
        if(dist.x() < 0)
          dist.setX(-1*dist.x());
        if(dist.y() < 0)
          dist.setY(-1*dist.y());
        if(dist.z() < 0)
          dist.setZ(-1*dist.z());

        //this is the sum of both box's radii
        vec3 sum = 0.5*(bmax - bmin + cmax - cmin);

        //if the dist is greater than the sum, then they
        //don't overlap
        if(dist.x() > sum.x()) continue;
        if(dist.y() > sum.y()) continue;
        if(dist.z() > sum.z()) continue;


        results.push_back(i);
    }

    return results;
}

std::vector<Octree *> Octree::getIntersections(Ray r) {

  std::vector<Octree*> results;

  unsigned char a = 0; // used for the label transformation function

  Ray rNew = r;
  
  if(r.direction.x() < 0.){

    int distToOrigin = r.origin.x() - o_origin.x();
    rNew.origin.setX(r.origin.x() - 2*distToOrigin);

    rNew.direction.setX(-r.direction.x());

    a |= 4;
  }
  if(r.direction.y() < 0) {

    int distToOrigin = r.origin.y() - o_origin.y();
    rNew.origin.setY(r.origin.y() - 2*distToOrigin );

    rNew.direction.setY(-r.direction.y());

    a |= 2;
  }
  if(r.direction.z() < 0) {

    int distToOrigin = r.origin.z() - o_origin.z();
    rNew.origin.setZ(r.origin.z() - 2*distToOrigin);

    rNew.direction.setZ(-r.direction.z());

    a |= 1;
  }

  float divx = 1 / rNew.direction.x();
  float divy = 1 / rNew.direction.y();
  float divz = 1 / rNew.direction.z();

  //ray point interesection, solved for t
  vec3 t0 = o_min - rNew.origin;
  // we need to divide this by d
  t0.setX(t0.x() * divx);
  t0.setY(t0.y() * divy);
  t0.setZ(t0.z() * divz);


  vec3 t1 = o_max - rNew.origin;
  t1.setX(t1.x() * divx);
  t1.setY(t1.y() * divy);
  t1.setZ(t1.z() * divz);

  float tmin = fmax(t0.x(),t0.y());
  tmin = fmax(tmin, t0.z());

  float tmax = fmin(t1.x(),t1.y());
  tmax = fmin(tmax, t1.z());

  if(tmin < tmax) {
    // check to see if the entire tree was even hit
    getSubtreeIntersection(t0,t1, a, results);
  }

  return results;
}


void Octree::getSubtreeIntersection(vec3 t0, vec3 t1, const unsigned char a, std::vector<Octree *> &results) {

  vec3 tm;
  int currNode;


  //WHY IS THIS HERE??????
  // I guess negative times means behind the image plane?
  if(t1.x() < 0. || t1.y() < 0. || t1.z() < 0.) {
    return;
  }

  if(isLeaf()) {
    results.push_back(this);
    return;
  }

  tm = (t0 + t1) * 0.5;

  currNode = firstNode(t0, tm);

  /*
   * This switch statement works to figure out the traversal
   * of a ray through the children of this particular node.
   * It workds under the assumption that all components of the
   * ray's direction vector is positive. We're using the xor
   * statements while indexing children to account for negative
   * direction components. (a was determined in getIntersections).
   * This was all taken from (Revelles et. al. 2000)
   */
  do {
    // values of 8 mean the ray exited the subtree
    switch (currNode) {
      case 0:
        o_children[a]->getSubtreeIntersection(t0, tm, a, results);
        currNode = nextNode(tm.x(), 4, tm.y(), 2, tm.z(), 1);
        break;
      case 1:
        o_children[1^a]->getSubtreeIntersection(vec3(t0.x(), t0.y(), tm.z()),
                                                vec3(tm.x(), tm.y(), t1.z()), a, results);
        currNode = nextNode(tm.x(), 5, tm.y(), 3, t1.z(), 8);
        break;
      case 2:
        o_children[2^a]->getSubtreeIntersection(vec3(t0.x(), tm.y(), t0.z()),
                                                vec3(tm.x(), t1.y(), tm.z()), a, results);
        currNode = nextNode(tm.x(), 6, t1.y(), 8, tm.z(), 3);
        break;
      case 3:
        o_children[3^a]->getSubtreeIntersection(vec3(t0.x(), tm.y(), tm.z()),
                                                vec3(tm.x(), t1.y(), t1.z()), a, results);
        currNode = nextNode(tm.x(), 7, t1.y(), 8, t1.z(), 8);
        break;
      case 4:
        o_children[4^a]->getSubtreeIntersection(vec3(tm.x(), t0.y(), t0.z()),
                                                vec3(t1.x(), tm.y(), tm.z()), a, results);
        currNode = nextNode(t1.x(), 8, tm.y(), 6, tm.z(), 5);
        break;
      case 5:
        o_children[5^a]->getSubtreeIntersection(vec3(tm.x(), t0.y(), tm.z()),
                                                vec3(t1.x(), tm.y(), t1.z()), a, results);
        currNode = nextNode(t1.x(), 8, tm.y(), 7, t1.z(), 8);
        break;
      case 6:
        o_children[6^a]->getSubtreeIntersection(vec3(tm.x(), tm.y(), t0.z()),
                                                vec3(t1.x(), t1.y(), tm.z()), a, results);
        currNode = nextNode(t1.x(), 8, t1.y(), 8, tm.z(), 7);
        break;
      case 7:
        o_children[7^a]->getSubtreeIntersection(tm, t1, a, results);
        currNode = 8;
        break;
    }
  } while(currNode < 8);

}


/* this uses the information contained within tables 1 and 2
 * from (Revelles et. al. 2000)
 */
int Octree::firstNode(vec3 t0, vec3 tm) {

  int node = 0;

  float tmin = fmax(t0.x(), t0.y());
  tmin = fmax(tmin, t0.z());

  if (tmin == t0.z()) {
    // Entry plane = XY
    if(tm.x() < t0.z())
      node |= 1;
    if(tm.y() < t0.z())
      node |= 2;
  } else if (tmin == t0.y()) {
    // Entry plane = XZ
    if(tm.x() < t0.y())
      node |= 1;
    if(tm.z() < t0.y())
      node |= 4;
  } else if (tmin == t0.x()) {
    // Entry plane = YZ
    if(tm.y() < t0.x())
      node |= 2;
    if(tm.z() < t0.x())
      node |= 4;
  }

  return node;

  // TODO: double check this ^^^^^^^^^^^^
}


/* this implements the information from table 3
 * from (Revelles et. al. 2000)
 * The values of n1-3 are the indices of the next
 * node depending on what the exit plane is
 */
int Octree::nextNode(float f1, int n1, float f2, int n2, float f3, int n3) {

    float max = fmin(f1, f2);
    max = fmin(max, f3);

    // this means the exit plan is YZ
    if(max == f1)
        return n1;

    // exit plane is XZ
    else if(max == f2)
        return n2;

    // exit plane is XY
    else if(max == f3)
        return n3;


    return 10000; //should never get here
}

std::vector<Shape*> Octree::getShapes() {
    std::vector<Shape*> result;

    for(int i=0; i<o_numShapes; i++) {
        result.push_back(o_shapes[i]);
    }

    return result;
}
