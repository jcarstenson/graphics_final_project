#ifndef CS40RGBIMAGE_H
#define CS40RGBIMAGE_H

#include <string>
#include "rgbColor.h"

namespace cs40{

class RGBImage{
public:
    /*Create a new RGBImage from existing file name
      (currently only PNG supported)*/
    RGBImage(const std::string& fname);

    /*Create a dummy RGBImage with no rows or columns,
     *use init to allocate space*/
    RGBImage(): m_height(0), m_width(0), m_pixels(NULL) {
      /* do nothing */
    };

    /*Create a new RGBImage with specified size and background Color
      (default black)*/
    RGBImage(int height, int width,
             const RGBColor& background=RGBColor(0,0,0)){
      init(height, width, background);
    }

    /*Destructor*/
    ~RGBImage();


    /*Create a new buffer with height and width,
     *deleting old buffer if it exists*/
    void init(int height, int width,
             const RGBColor& background=RGBColor(0,0,0));

    /*Return height (number of rows)*/
    inline int rows(){ return m_height; }
    
    /*Return width (number of columns)*/
    inline int cols(){ return m_width; }

    /* Return a reference to a pixel color.
     * This pixel value can be changed through the reference, e.g.,
     * img(row,col)=color
     * method throws a range_error if indices are out of bounds
     * 0,0 is upper left of image*/
    RGBColor& operator() (int row, int col);
    
    /*set all pixel values to given background color*/
    void clear(const RGBColor& background=RGBColor(0,0,0));

    /* Save current image buffer to output file.
       To overwrite existing file, explicitly set overwrite to true
       Return true if successfully saved, and false otherwise
      (currently only PNG supported)*/
    bool saveAs(const std::string& fname, bool overwrite=false);

private:
    //Allocate space for pixels
    void makePixels();
    
    int m_height, m_width;
    RGBColor ** m_pixels;
};

} //namespace
#endif 
