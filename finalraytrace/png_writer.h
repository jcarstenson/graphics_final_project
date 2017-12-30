#ifndef CS40PNG_WRITER_H
#define CS40PNG_WRITER_H

#include <png.h>
#include <string>
#include "rgbColor.h"

namespace cs40{

class png_writer {
public:

    /*Attempt to write a png file to fname with given
     *height and width, overwriting existing file if overwrite is
     *set to true. Check isValid after calling constructor to see
     *if you can successfully write to file. Trying to create a
     *overwrite an existing file without overwrite set to true
     *throws a runtime_error
     */
    png_writer(const std::string& fname,
               int height, int width, bool overwrite=false);

    /*destructor*/
    ~png_writer();

    /*Return true if object is valid and ready to write,
     *Return false otherwise*/
    inline bool isValid(){ return m_valid; }

    /*Return height (number of rows)*/
    inline int rows(){ return m_nrows; }

    /*Return width (number of columns)*/
    inline int cols(){ return m_ncols; }

    /*set the color of the given row and col to color
     *throw range_error if indices are out of bounds
     * throw runtime_error if image is invalid
     */
    void setColor(int row, int col, const RGBColor& color);

    /*explicity close image and write to disk. causes future
     *calls to isValid to return false and calls to setColor to
     *throw runtime_error
     */
    void close();

private:
    int m_ncols, m_nrows;
    bool m_valid;
    FILE* m_fp;
    png_byte m_color_type;
    png_byte m_bit_depth;
    png_structp m_png_ptr;
    png_infop m_info_ptr;
    png_bytep* m_row_buffers;
};

}//namespace
#endif
