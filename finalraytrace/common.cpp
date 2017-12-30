#include "common.h"

using std::ostream;

ostream& operator<<(ostream& str, const QVector3D& vec){
    str << (float) vec.x() << ", "
        << (float) vec.y() << ", "
        << (float) vec.z();
    return str;
}

