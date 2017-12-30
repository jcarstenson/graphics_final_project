#ifndef CS40RAYTRACER_H
#define CS40RAYTRACER_H

#include <vector>
#include <string>
#include <QHash>      //Qt Dictionary
#include <QString>    //Appease QT with its strings
#include "scene.h"
#include "material.h"
#include "rgbColor.h"
#include "rgbImage.h"

namespace cs40{

class RayTracer{

public:
    RayTracer();
    ~RayTracer();

    /* parse an input file given by a filename and build the global
     * scene object from the parsed input */
    void parseFile(const std::string& fname);

    /* Do the raytracing */
    void trace();

    /* Save output image */
    void save();

private:
    cs40::Scene m_scene;
    QHash<QString, vec3> m_colors;
    cs40::RGBImage m_img;
    bool usingOct;

    /* Hash table mapping material names to material structs.
     * materials["current_"] is a special entry refering to the
     * current material. The current material is applied to all newly
     * created objects */
    QHash<QString, cs40::Material> m_materials;

    /* parse a single line in the input file consisting of the given
     * vector of whitespace-delimited words. Non comment non blank lines
     * are of the form <commandname> [arguments] */
    void parseLine(const std::vector<std::string>& words);

    /* parse a material command in the vector words,
     * using m_materials to load/store/modify current and other maps */
    void parseMat(const std::vector<std::string>& words);

    /* convert from 0-1 rgb space to 0-255 */
    cs40::RGBColor convertColor(const vec3& clr);

    /* function to find the actual size (width, height) of a pixel
     * which will be contained within the View object */
    void findPixelSize(View* v);

    /* function to find the actual location of an individual pixel */
    std::vector<vec3> pxlPts(View v, int i, int j, int rays);

    /* function to take a ray and find the colosest object and
     * return that object */
    Shape* findObject(Ray r);

    /* function to take a ray and the closest object to find out
     * what color the pixel should be */
    vec3 findColor(Ray r, int depth);

    /* function to determine if a point is shadowed by a light */
    bool isShadow(vec3 p, vec3 l);

    /* Function to mix colors when we do reflections */
    vec3 mixColor(vec3 c1, vec3 c2, float shiny);

    void loadBar(int i, int j, int nI, int nJ);

    vec3 findColorOct(Octree* o, Ray r, int depth);

    Shape* findObjectOct(Octree* o, Ray r);

};

}
#endif // RAYTRACER_H
