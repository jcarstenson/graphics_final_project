#include "ray.h"
#include <sstream>

using std::string;
using std::ostream;
using std::stringstream;
using cs40::Ray;

Ray::Ray():origin(0,0,0),direction(0,0,-1){ };

Ray::Ray(const Ray& other):
    origin(other.origin), direction(other.direction) { };

Ray::Ray(const vec3& src, const vec3& dir):
    origin(src), direction(dir){ };

/* convert Ray to string representation */
string Ray::str() const {
    stringstream ss;
    ss << "src: " << origin << " dir: " << direction;
    return ss.str();
}

/* allow cout << ray */
ostream& operator<<(ostream& os, const Ray& r){
    return os << r.str();
}

