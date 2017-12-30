#ifndef CS40COMMON_H
#define CS40COMMON_H

#include <QVector3D>
#include <ostream>

typedef QVector3D vec3;

/* Print QVector3d objects using cout. */
std::ostream& operator<<(std::ostream& str, const QVector3D& vec);


#endif 
