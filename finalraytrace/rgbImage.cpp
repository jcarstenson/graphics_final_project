#include "rgbImage.h"
#include "png_reader.h"
#include "png_writer.h"
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <sstream>

using std::string;
using std::stringstream;
using std::endl;
using std::cout;
using std::runtime_error;
using std::range_error;
using cs40::RGBImage;
using cs40::RGBColor;
using cs40::png_reader;
using cs40::png_writer;


RGBImage::RGBImage(const std::string& fname){
    m_height=0;
    m_width=0;
    png_reader img(fname);
    if(img.isValid()){
        m_height=img.rows();
        m_width=img.cols();
        makePixels();
        for(int c=0; c<m_width; c++){
            for(int r=0; r<m_height; r++){
                m_pixels[r][c]=img(r,c);
            }
        }
    }
}


void RGBImage::init(int height, int width, const RGBColor& background){
    if(m_pixels){
      for(int i=0; i<m_height; i++){
        delete [] m_pixels[i]; m_pixels[i]=NULL;
      }
      delete [] m_pixels; m_pixels=NULL;
    }
    m_height=height;
    m_width=width;
    makePixels();
    clear(background);
}


RGBImage::~RGBImage(){
    for(int row=0; row<m_height; row++){
        delete [] m_pixels[row];
    }
    delete [] m_pixels;
}

void RGBImage::clear(const RGBColor& background){
    for(int row=0; row<m_height; row++){
        for(int col=0; col<m_width; col++){
            m_pixels[row][col]=background;
        }
    }
}

RGBColor& RGBImage::operator()(int row, int col){
    stringstream emsg;
    if(col < 0 || col > m_width){
        emsg << "Column index " << col << " out of bounds" << endl;
        throw range_error(emsg.str());
    }
    if(row < 0 || row > m_height){
        emsg << "Row index " << row << " out of bounds" << endl;
        throw range_error(emsg.str());
    }

    return m_pixels[row][col];
} 


void RGBImage::makePixels(){
    if(m_height<=0){ return; }
    if(m_width<=0){ return; }
    m_pixels=new RGBColor*[m_height];
    for(int i=0; i<m_height; i++){
        m_pixels[i]=new RGBColor[m_width];
    }
}

bool RGBImage::saveAs(const std::string& fname, bool overwrite){
    try{
        png_writer outimg(fname, m_height, m_width, overwrite);
        for(int r=0; r<m_height; r++){
            for(int c=0; c<m_width; c++){
                outimg.setColor(r,c,m_pixels[r][c]);
            }
        }
        outimg.close();
        return true;
    }
    catch (runtime_error e){
        cout << "Error saving image to " << fname << ": " << e.what() << endl;
    }
    return false;
}

