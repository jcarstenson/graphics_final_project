#ifndef CS40RGBCOLOR_H
#define CS40RGBCOLOR_H

#include <stdexcept>
#include <sstream>
#include <QVector3D>

namespace cs40{
typedef struct rgbcolor_s{
    //struct is just a public class in C++
    unsigned char r;
    unsigned char g;
    unsigned char b;

    //struct Constructors allowed in C++
    //Default (black) constructor
    rgbcolor_s(): r(0), g(0), b(0) {};

    rgbcolor_s(unsigned char red, unsigned char green, unsigned char blue):
        r(red), g(green), b(blue){};

    //copy constructor
    rgbcolor_s(const struct rgbcolor_s& other){
        r=other.r; g=other.g; b=other.b;
    }

    /* Convert from 0-255 RGBColor to 0-1 QVector3D */
    QVector3D toVec3() const { return QVector3D(r/255.,g/255.,b/255); }

    /* Set RGB 0-255 components from 0-1 QVector3D */
    void fromVec3(const QVector3D& clr){
      r=static_cast<int>(255*clr.x());
      g=static_cast<int>(255*clr.y());
      b=static_cast<int>(255*clr.z());
    }


    //Allow color(0) syntax for returning red component (1,2 for green,blue)
    unsigned char& operator()(int idx){
        std::stringstream emsg;
        if(idx < 0 || idx > 2){
            emsg << "rgb index " << idx << " out of bounds" << std::endl;
            throw std::range_error(emsg.str());
        }
        if(idx==0){ return r; }
        else if(idx==1){ return g; }
        return b;
    }


} RGBColor;

}//namespace

#endif 
