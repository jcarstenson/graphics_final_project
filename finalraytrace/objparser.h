#ifndef CS40OBJPARSER_H
#define CS40OBJPARSER_H

//includes
#include <string>
#include <vector>
#include "common.h"
#include <QHash>
#include "triangle.h"


namespace cs40{

class ObjParser{

public:
    ObjParser();
    ~ObjParser();

    void parseObj(const std::string& fname);

    inline std::vector<cs40::Triangle*> getObjects() {return tList;}


private:
    int index;
    QHash<int, vec3> vList;
    std::vector<cs40::Triangle*> tList;

    void parseObjLine(const std::vector<std::string>& words);

};
}
#endif // CS40OBJPARSER_H
