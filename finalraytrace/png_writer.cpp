#include "png_writer.h"
#include <cstdio>
#include <stdexcept>
#include <iostream>

using std::string;
using std::stringstream;
using std::endl;
using std::cout;
using std::runtime_error;
using std::range_error;
using cs40::png_writer;
using cs40::RGBColor;


png_writer::png_writer(const string& fname,	
                       int height, int width, bool overwrite){
    m_valid = false;
    m_ncols = m_nrows = 0; //delay until we know img is valid

    stringstream emsg;

    if(!overwrite){ /* check if file exists */
        m_fp = fopen(fname.c_str(), "rb");
        if (m_fp){
            emsg << fname << " exists (and overwrite is false)"
                 << endl;
            fclose(m_fp);
            throw runtime_error(emsg.str());
        }
    }

    m_fp = fopen(fname.c_str(), "wb");
    if (!m_fp){
        emsg << "Cannot open " << fname << " for writing " << endl;
        throw runtime_error(emsg.str());
    }

    /* initialize metadata */
    m_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                        NULL, NULL, NULL);

    if (!m_png_ptr){
        fclose(m_fp);
        throw runtime_error("png_create_write_struct failed");
    }

    m_info_ptr = png_create_info_struct(m_png_ptr);
    if (!m_info_ptr){
        fclose(m_fp);
        throw runtime_error("png_create_info_struct failed");
    }

    if (setjmp(png_jmpbuf(m_png_ptr))){
        fclose(m_fp);
        throw runtime_error("Error during png_init_io");
    }
    png_init_io(m_png_ptr, m_fp);


    m_nrows = height;
    m_ncols = width;
    m_bit_depth = 8; //8 bits per channel 0-255
    m_color_type = PNG_COLOR_TYPE_RGB; //Only RGB support for now

    /* write header */
    if (setjmp(png_jmpbuf(m_png_ptr))){
        fclose(m_fp);
        throw runtime_error("Error writing png header");
    }

    png_set_IHDR(m_png_ptr, m_info_ptr, m_ncols, m_nrows,
                 m_bit_depth, m_color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(m_png_ptr, m_info_ptr);

    /* Allocate space */

    m_row_buffers = new png_bytep[m_nrows];
    int bytes_per_row = png_get_rowbytes(m_png_ptr,m_info_ptr);
    for (int y=0; y<m_nrows; y++){
        m_row_buffers[y] = new png_byte[bytes_per_row];
        for(int k=0; k<bytes_per_row; k++){
            m_row_buffers[y][k]=(png_byte)0;
        }
    }

    /* Ready to go */
    m_valid=true;
}

png_writer::~png_writer(){
    if(m_fp){
        //Forgot to run close. Try to do so, but destructors
        //can't throw exceptions
        /* write bytes */
        if (setjmp(png_jmpbuf(m_png_ptr))){
            cout << "Error writing png bytes in destructor";
        }
        png_write_image(m_png_ptr, m_row_buffers);
        /* end write */
        if (setjmp(png_jmpbuf(m_png_ptr))){
            cout << "Error writing png finish bytes in destructor";
        }
        png_write_end(m_png_ptr, NULL);
        fclose(m_fp);
        m_fp = NULL;
    }

    for(int y=0; y<m_nrows; y++){
        delete [] m_row_buffers[y];
        m_row_buffers[y]=NULL;
    }
    delete [] m_row_buffers; m_row_buffers=NULL;
}

void png_writer::setColor(int row, int col, const RGBColor& color){
    stringstream emsg;
    if(col < 0 || col > m_ncols){
        emsg << "Column index " << col << " out of bounds" << endl;
        throw range_error(emsg.str());
    }
    if(row < 0 || row > m_nrows){
        emsg << "Row index " << row << " out of bounds" << endl;
        throw range_error(emsg.str());
    }
    if(!m_valid){
        throw runtime_error("PNG image not valid\n");
    }

    png_bytep row_buf = m_row_buffers[row];
    //Assumes RGB 3 bytes per pixel
    png_byte* pix_ptr = &row_buf[col*3];


    pix_ptr[0]=color.r;
    pix_ptr[1]=color.g;
    pix_ptr[2]=color.b;
} 

void png_writer::close(){
    /* write bytes */
    if (setjmp(png_jmpbuf(m_png_ptr))){
        throw runtime_error("Error writing png bytes");
    }

    png_write_image(m_png_ptr, m_row_buffers);
    /* end write */
    if (setjmp(png_jmpbuf(m_png_ptr))){
        throw runtime_error("Error writing png finish bytes");
    }
    png_write_end(m_png_ptr, NULL);
    fclose(m_fp);
    m_fp = NULL;
    m_valid = false;
}
