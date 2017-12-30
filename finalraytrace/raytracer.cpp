//C++ STL
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <time.h>

//QT
#include <QHash>      //Qt Dictionary
#include <QString>    //Appease QT with its strings

//Our stuff
#include "raytracer.h"
#include "view.h"     //image, eye properties
#include "shape.h"    //base class for hitable objects
#include "light.h"    //basic light struct (pos, intensity)
#include "parser.h"   //helper functions for reading input file
#include "ray.h"      //things we trace
#include "scene.h"    //things we hit
#include "rgbImage.h" //things to which we save output
#include "objparser.h" //things to parse obj file with

//our shapes
#include "sphere.h"
#include "plane.h"
#include "rectangle.h"
#include "triangle.h"


using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::vector;

using cs40::RayTracer;
using cs40::RGBColor;
using cs40::Material;
using cs40::Scene;
using cs40::View;
//get stuck? use the nuclear option..
//using namespace cs40;


RayTracer::RayTracer(){
    m_img = RGBImage();
    m_scene.rays = 1;
    m_scene.depth = 1; //default rays and depth values
    usingOct = false;
    m_materials.insert("current_", Material());
}

RayTracer::~RayTracer(){
    while(!m_scene.objects.empty()){
        delete m_scene.objects.back();
        m_scene.objects.pop_back();
    }
}

void RayTracer::trace(){

    // insert all of our shapes into the octree
    if(usingOct) {
        int n = m_scene.objects.size();
        for(int i=0; i<n; i++) {
            m_scene.oct->insert(m_scene.objects[i]);
        }
    }

    findPixelSize(&m_scene.view);
    View vw = m_scene.view;

    m_img.init(vw.nrows, vw.ncols, convertColor(vw.background));

    // let the ray tracing begin!!
    for(int i=0; i < vw.nrows; i++) {

        //print the status
        loadBar(i, vw.ncols, vw.nrows, vw.ncols);

        for(int j=0; j < vw.ncols; j++) {

            // print the status
            //loadBar(i, j, vw.nrows, vw.ncols);

            vec3 avgColors = vec3(0.,0.,0.);

            // get all of the points each ray should point to
            std::vector<vec3> pxls = pxlPts(m_scene.view, i, j, m_scene.rays);

            // ray trace each one
            int nPxls = pxls.size();
            while(!pxls.empty()) {

                vec3 pxl = pxls.back();
                pxls.pop_back();

                // construct the ray
                vec3 dir = (pxl - vw.eye).normalized();
                Ray r = Ray(vw.eye, dir);

                vec3 color;

                if(!usingOct) {
                    color = findColor(r, m_scene.depth);
                } else {
                    color = findColorOct(m_scene.oct, r, m_scene.depth);
                }

                //add the found color to the average color value
                avgColors += color;
            }

            //divide to get the true average
            avgColors /= nPxls;
            m_img(i,j) = convertColor(avgColors);
        }
    }

    cout << endl;

}

void RayTracer::save(){
    if(m_img.rows()==0){
      cout << "Image not initialized. Not saving" << endl;
      return;
    }
    m_img.saveAs(m_scene.view.fname, true);
    cout << "Saved result to " << m_scene.view.fname << endl;
}

/* convert from 0-1 rgb space to 0-255 */
RGBColor RayTracer::convertColor(const vec3& clr){
    int r,g,b;
    r=(int)(255*clr.x());
    g=(int)(255*clr.y());
    b=(int)(255*clr.z());
    return RGBColor(r,g,b);
}

void RayTracer::parseFile(const string& fname){
    ifstream infile;
    infile.open(fname.c_str());

    if(!infile.good()){
        cout << "error opening file " << fname << endl;
        return;
    }

    int lno = 1; //line number
    string line; //current line
    vector<string> words;


    while(!infile.eof()){
        getline(infile, line);
        //cout << lno << ": " << line.length() << " " << line <<  endl;
        words = split(line);
        if(words.size()>0){
            try{
                parseLine(words);
            }
            catch(parser_error e){
                cout << "Error on line " << lno << ": " << line << endl;
                cout << e.what() << endl;
            }
        }
        lno++;
    }

    infile.close();
}

void RayTracer::parseLine(const vector<string>& words){
    string cmd = words[0];
    if(cmd == "output"){
        checksize(words,1);
        m_scene.view.fname = words[1];
    }
    else if (cmd == "outsize"){
        checksize(words,2);
        m_scene.view.nrows = parseInt(words[1]);
        m_scene.view.ncols = parseInt(words[2]);
    }
    else if (cmd == "eye"){
        checksize(words,3);
        m_scene.view.eye = parseVec3(words,1);
    }
    else if (cmd == "background"){
        checksize(words,3);
        m_scene.view.background = parseVec3(words,1);
    }
    else if (cmd == "color"){
        checksize(words,4);
        //add named color to map
        m_colors[words[1].c_str()]=parseVec3(words,2);
    }
    else if (cmd == "mat"){
        //either mat cmd name or mat cmd r g b
        //or mat shiny float
        checksize(words,2,4);
        parseMat(words); //this gets complicated
    }
    else if (cmd == "sphere"){
        checksize(words, 4);
        vec3 center = parseVec3(words,1);
        float radius = parseFloat(words[4]);
        m_scene.objects.push_back(new Sphere(center, radius));
        m_scene.objects.back()->material = m_materials["current_"];
    }
    else if (cmd == "rectangle"){
        checksize(words, 9);
        vec3 ll = parseVec3(words,1);
        vec3 lr = parseVec3(words,4);
        vec3 ur = parseVec3(words,7);
        m_scene.objects.push_back(new Rectangle(ll, lr, ur));
        m_scene.objects.back()->material = m_materials["current_"];
    }
    else if (cmd == "triangle"){
        checksize(words, 9);
        vec3 ll = parseVec3(words,1);
        vec3 lr = parseVec3(words,4);
        vec3 top = parseVec3(words,7);
        m_scene.objects.push_back(new Triangle(ll, lr, top));
        m_scene.objects.back()->material = m_materials["current_"];
    }
    else if (cmd == "amblight"){
        checksize(words,1);
        m_scene.ambient = parseFloat(words[1]);
    }
    else if (cmd == "light"){
        checksize(words,4);
        Light l;
        l.position = parseVec3(words, 1);
        l.intensity = parseFloat(words[4]);
        m_scene.lights.push_back(l);
    }
    else if (cmd == "origin") {
        checksize(words, 3);
        m_scene.view.origin = parseVec3(words, 1);
    }
    else if (cmd == "horiz") {
        checksize(words, 3);
        m_scene.view.horiz = parseVec3(words, 1);
    }
    else if (cmd == "vert") {
        checksize(words, 3);
        m_scene.view.vert = parseVec3(words, 1);

    }
    else if (cmd == "rays") {
        checksize(words, 1);
        m_scene.rays = parseInt(words[1]);
    }
    else if (cmd == "depth") {
        checksize(words, 1);
        m_scene.depth = parseInt(words[1]);
    }
    else if (cmd == "obj") {
        checksize(words,1);
        ObjParser parser;
        parser.parseObj(words[1]);
        vector<cs40::Triangle*> triangles = parser.getObjects();
        for(int i = 0; i < triangles.size(); i++) {
            m_scene.objects.push_back(triangles[i]);
            m_scene.objects.back()->material = m_materials["current_"];
        }
    }
    else if (cmd == "oct") {
        checksize(words,5);
        vec3 o = parseVec3(words,1);
        float halfDim = parseFloat(words[4]);
        int maxData = parseInt(words[5]);

        m_scene.oct = new cs40::Octree(o, halfDim, maxData);
        usingOct = true;
    }
    else{
        throw parser_error("Unknown command: "+cmd);
    }
}

/* parse a material command in the vector words, 
 * using matmap to load/store/modify current and other maps */
void RayTracer::parseMat(const vector<string>& words){
    string subcmd = words[1];
    if(subcmd == "save"){
        m_materials[words[2].c_str()] = m_materials["current_"];
    }
    else if(subcmd == "load"){
        if(m_materials.contains(words[2].c_str())){
            m_materials["current_"]=m_materials[words[2].c_str()];
        }
        else{
            throw parser_error("No Material " + words[2] + " found");
        }
    }
    else if(subcmd != "amb" && subcmd != "diff" && subcmd != "spec" && subcmd != "shiny"){
        throw parser_error("Unknown material subcommand: " + subcmd);
    }
    else{
        //looks like mat <type> <name> or mat <type> r g b
        //where <type> is amb, diff, or spec or shiny
        vec3 clr;
        if(words.size()>3 ){ //mat <type> r g b
            clr = parseVec3(words, 2);
        }
        else if (subcmd != "shiny"){ //mat <type> name, where name refers to existing color
            string clrname = words[2];
            if(m_colors.contains(clrname.c_str())){
                clr=m_colors[clrname.c_str()];
            }
            else{
                throw parser_error("No color " + clrname + " found");
            }
        }
        if(subcmd=="amb"){m_materials["current_"].ambient=clr;}
        else if(subcmd=="diff"){m_materials["current_"].diffuse=clr;}
        else if(subcmd=="spec"){m_materials["current_"].specular=clr;}
        else if(subcmd=="shiny"){m_materials["current_"].shiny=parseFloat(words[2]);}
    }
}


void RayTracer::findPixelSize(cs40::View* v) {

    float rows = v->nrows;
    float cols = v->ncols;

    float height = v->vert.length();
    float width = v->horiz.length();

    v->pxlHeight = height / rows;
    v->pxlWidth = width / cols;
}

std::vector<vec3> RayTracer::pxlPts(View v, int i, int j, int rays) {

    std::vector<vec3> pxls;

    srand (time(NULL)); // setting the random seed

    float pxlWidth = v.pxlWidth;
    float pxlHeight = v.pxlHeight;

    // the defined origin here is the upper left corner
    vec3 o = v.origin+v.vert;

    // the pixel width vector is the horiz vector
    // but with the magnitude of pxlWidth
    vec3 width = v.horiz;
    width.normalize();
    width *= pxlWidth;

    // same for height, except it is in the opposite
    // direction of vert
    vec3 height = -v.vert;
    height.normalize();
    height *= pxlHeight;


    // i demarcs the row #, j is the col #
    vec3 pxl = o + i*height + j*width;

    // now we add a random amount of height and width
    // less than the height,width of a pixel to make
    // each ray slightly different to improve anti-
    // aliasing
    for(int i=0; i < rays; i++) {
        vec3 randPxl;

        float randW = (float)rand()/ (float)RAND_MAX;
        float randH = (float)rand()/ (float)RAND_MAX;

        randPxl = pxl + randW*width;
        randPxl = pxl + randH*height;

        pxls.push_back(randPxl);
    }

    return pxls;
}



cs40::Shape* RayTracer::findObject(Ray r) {

    //pair1 = index of object, pair2 = hit time
    std::vector< QPair< int, float > > hitShapes;

    int nShapes = m_scene.objects.size();

    for(int i = 0; i < nShapes; i++) {
        float t = m_scene.objects.at(i)->hitTime(r);

        // list of all the hit objects
        if (t >= 0.0001) { // makes sure it doesn't return the object itself
            hitShapes.push_back(QPair<int,float>(i,t));
        }
    }

    if(hitShapes.empty()) {
        return NULL;
    }

    // now we find the lowest hit time
    // and "current" will be the closest object
    QPair<int,float> current = hitShapes.back();
    hitShapes.pop_back();
    while(!hitShapes.empty()) {
        if(current.second > hitShapes.back().second) {
            current = hitShapes.back();
            hitShapes.pop_back();
        } else {
            hitShapes.pop_back();
        }
    }

    return m_scene.objects[current.first];
}


vec3 RayTracer::findColor(Ray ray, int depth) {

    // get the closest object from the ray
    Shape* o = findObject(ray);
    vec3 color = m_scene.view.background;
    if (o != NULL) {

        float t = o->hitTime(ray);

        //Point of intersection
        vec3 p = ray(t);

        //View vector
        vec3 v = (ray.origin - p).normalized();

        //Normal vector
        vec3 n = o->normal(p);

        int nLights = m_scene.lights.size();
        bool shadows[nLights];
        for(int i=0; i < nLights; i++) {
           vec3 lightPos = m_scene.lights[i].position;
           shadows[i] = isShadow(p, (lightPos-p));
        }

        //Calculate color change due to ambient light
        float La = m_scene.ambient;
        vec3 Ma = o->material.ambient;
        vec3 Ia = La*Ma;

        //Calculate color change due to diffuse light
        vec3 Md = o->material.diffuse;
        float Ld;
        vec3 IdTemp, Id, l;
        IdTemp = vec3(0.,0.,0.);
        Id = vec3(0.,0.,0.);
        for(int i = 0; i < nLights; i++){
            Ld = m_scene.lights[i].intensity;
            l = (m_scene.lights[i].position - p);
            // Is it blocked by an object?
            if(shadows[i]) {
                continue;
            }

            l.normalize();

            IdTemp = Ld*Md*(fmax(QVector3D::dotProduct(n,l), 0.));
            Id = Id + IdTemp;
        }

        //Calculate color change due to specular light
        vec3 Ms = o->material.specular;
        float Ls;
        vec3 IsTemp, Is, r;
        Is = vec3(0.,0.,0.);
        for(int i = 0; i < nLights; i++){
            Ls = m_scene.lights[i].intensity;
            l = (m_scene.lights[i].position - p);

            // Is it blocked by an object?
            if(shadows[i]) {
               continue;
            }

            l.normalize();

            r = (2*(vec3::dotProduct(l,n)*n)) - l;
            float dotProd = pow(fmax(QVector3D::dotProduct(r,v), 0.), 5);

            IsTemp = Ls*Ms*(fmax(dotProd, 0.));
            Is = Is + IsTemp;
        }

        color = Ia + Id + Is;

        //Reflection calculations
        r = ray.direction - 2*(vec3::dotProduct(ray.direction,n)*n);
        Ray rRay(p, r);
        if(o->material.shiny>0 && (depth > 0) && findColor(rRay,depth-1)!=vec3(0.,0.,0.)){
            color = mixColor(color, findColor(rRay, depth-1), o->material.shiny);
        }

        //Check to see if we're going over 1.0 in any color aspect
        if(color.x()>1.0){
            color.setX(1.0);
        }
        if(color.y()>1.0){
            color.setY(1.0);
        }
        if(color.z()>1.0){
            color.setZ(1.0);
        }

    } //end if not null

    return color;
}


bool RayTracer::isShadow(vec3 p, vec3 l) {

    cs40::Ray lightRay = cs40::Ray(p, l);

    cs40::Shape* closest = findObject(lightRay);

    if(closest != NULL) {
        if((closest->hitTime(lightRay) < 1) && (closest->hitTime(lightRay) > 0.001)){
            // the object is in front of the light
            return true;
        } else {
            // the object is behind the light
            return false;
        }
    } else {
        // no object to find
        return false;
    }
}

vec3 RayTracer::mixColor(vec3 c1, vec3 c2, float shiny){
    return (c1*(1-shiny) + (c2*shiny));
}


void RayTracer::loadBar(int i, int j, int nI, int nJ) {
    int totPix = nI*nJ;

    int pixDone = i*nJ + j;

    float ratio = ((float)pixDone/(float)totPix);
    int c = ratio*60;

    float percent = c/60.*100;

    printf("%.2f%% [", percent);
    for(int i=0; i<c; i++) cout << "=";
    for(int i=c; i<60; i++) cout << " ";
    cout << "]\r";
    cout.flush();
}

cs40::Shape* RayTracer::findObjectOct(cs40::Octree *o, Ray r){

    vector<cs40::Shape*> shapes;

    vector<cs40::Octree*> hitNodes = o->getIntersections(r);

    while(!hitNodes.empty()) {
        cs40::Octree* currOct = hitNodes.back();
        hitNodes.pop_back();

        vector<cs40::Shape*> currShapes = currOct->getShapes();

        while(!currShapes.empty()) {
            shapes.push_back(currShapes.back());
            currShapes.pop_back();
        }
    }

    //key is the objects index in shapes, value is its hit time
    QHash<int, float> hitShapes;

    int n = shapes.size();

    for(int i=0; i<n; i++) {
        float t = shapes[i]->hitTime(r);
        if (t >= 0.0001) { // makes sure it doesn't return the object itself
            hitShapes.insert(i, t);
        }
    }

    if(hitShapes.empty()){
        return NULL;
    }

    // now we find the lowest hit time
    // and "current" will be the closest object
    QList<float> values = hitShapes.values();
    float current = values.takeAt(0);
    while(!values.empty()){
        if(current > values.at(0)) {
            current = values.takeAt(0);
        } else {
            values.removeFirst();
        }
    }

    int index = hitShapes.key(current);

    return shapes[index];
}

vec3 RayTracer::findColorOct(Octree *oct, Ray ray, int depth){
    // get the closest object from the ray
    Shape* o = findObjectOct(oct,ray);
    vec3 color = m_scene.view.background;
    if (o != NULL) {

        float t = o->hitTime(ray);

        //Point of intersection
        vec3 p = ray(t);

        //View vector
        vec3 v = (ray.origin - p).normalized();

        //Normal vector
        vec3 n = o->normal(p);

        int nLights = m_scene.lights.size();
        bool shadows[nLights];
        for(int i=0; i < nLights; i++) {
           vec3 lightPos = m_scene.lights[i].position;
           shadows[i] = isShadow(p, (lightPos-p));
        }

        //Calculate color change due to ambient light
        float La = m_scene.ambient;
        vec3 Ma = o->material.ambient;
        vec3 Ia = La*Ma;

        //Calculate color change due to diffuse light
        vec3 Md = o->material.diffuse;
        float Ld;
        vec3 IdTemp, Id, l;
        IdTemp = vec3(0.,0.,0.);
        Id = vec3(0.,0.,0.);
        for(int i = 0; i < nLights; i++){
            Ld = m_scene.lights[i].intensity;
            l = (m_scene.lights[i].position - p);
            // Is it blocked by an object?
            if(shadows[i]) {
                continue;
            }

            l.normalize();

            IdTemp = Ld*Md*(fmax(QVector3D::dotProduct(n,l), 0.));
            Id = Id + IdTemp;
        }

        //Calculate color change due to specular light
        vec3 Ms = o->material.specular;
        float Ls;
        vec3 IsTemp, Is, r;
        Is = vec3(0.,0.,0.);
        for(int i = 0; i < nLights; i++){
            Ls = m_scene.lights[i].intensity;
            l = (m_scene.lights[i].position - p);

            // Is it blocked by an object?
            if(shadows[i]) {
               continue;
            }

            l.normalize();

            r = (2*(vec3::dotProduct(l,n)*n)) - l;
            float dotProd = pow(fmax(QVector3D::dotProduct(r,v), 0.), 5);

            IsTemp = Ls*Ms*(fmax(dotProd, 0.));
            Is = Is + IsTemp;
        }

        color = Ia + Id + Is;

        //Reflection calculations
        r = ray.direction - 2*(vec3::dotProduct(ray.direction,n)*n);
        Ray rRay(p, r);
        if(o->material.shiny>0 && (depth > 0) && findColorOct(oct,rRay,depth-1)!=vec3(0.,0.,0.)){
            color = mixColor(color, findColorOct(oct, rRay, depth-1), o->material.shiny);
        }

        //Check to see if we're going over 1.0 in any color aspect
        if(color.x()>1.0){
            color.setX(1.0);
        }
        if(color.y()>1.0){
            color.setY(1.0);
        }
        if(color.z()>1.0){
            color.setZ(1.0);
        }

    } //end if not null

    return color;
}







