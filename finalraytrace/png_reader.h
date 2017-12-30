#ifndef CS40PNG_READER_H
#define CS40PNG_READER_H

#include <png.h>
#include <string>
#include "rgbColor.h"

namespace cs40{

class png_reader {
public:

    /*Attempt to open an existing png file*/
    png_reader(const std::string& fname);

    /*destructor*/
    ~png_reader();
    
    /*Return a pixel color at a given row, col.
     throws a range_error if indices are out of bounds
     0,0 is upper left of image*/
    RGBColor operator() (int row, int col);

    /*Return true if object is valid and ready to read,
     *Return false otherwise*/
    inline bool isValid(){ return m_valid; }

    /*Return width (number of columns)*/
    inline int cols(){ return m_ncols; }

    /*Return height (number of rows)*/
    inline int rows(){ return m_nrows; }

private:
    int m_ncols, m_nrows;
    bool m_valid;
    png_byte m_color_type;
    png_byte m_bit_depth;
    png_structp m_png_ptr;
    png_infop m_info_ptr;
    png_bytep* m_row_buffers;
};

}
#endif
