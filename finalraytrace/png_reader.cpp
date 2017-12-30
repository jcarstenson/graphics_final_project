#include "png_reader.h"
#include <cstdio>
#include <stdexcept>

using std::string;
using std::stringstream;
using std::endl;
using std::runtime_error;
using std::range_error;
using cs40::png_reader;
using cs40::RGBColor;

png_reader::png_reader(const string& fname){
    m_valid = false;
    m_ncols = m_nrows = 0;

    stringstream emsg;
    png_byte header[8];  // 8 is the maximum size that can be checked
    /* open file and test for it being a png */
    FILE *fp = fopen(fname.c_str(), "rb");
    if (!fp){
        emsg << "Cannot open " << fname << endl;
        throw runtime_error(emsg.str());
    }


    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)){
        emsg << fname  << " does not look like a png file " << endl;
        fclose(fp);
        throw runtime_error(emsg.str());
    }


    /* initialize stuff */
    m_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                       NULL, NULL, NULL);

    if (!m_png_ptr){
        fclose(fp);
        string m = "[read_png_file] png_create_read_struct failed\n";
        throw runtime_error(m);
    }

    m_info_ptr = png_create_info_struct(m_png_ptr);
    if (!m_info_ptr){
        fclose(fp);
        string m = "[read_png_file] png_create_info_struct failed\n";
        throw runtime_error(m);
    }

    if (setjmp(png_jmpbuf(m_png_ptr))) {
        //Weirdo setjmp/lonjmp C-style exception handling
        //used by libpng. see setjmp.h
        fclose(fp);
        throw runtime_error("[read_png_file] Error during init_io\n");
    }

    png_init_io(m_png_ptr, fp);
    //Inform libpng that we already read the header
    png_set_sig_bytes(m_png_ptr, 8);
    png_read_info(m_png_ptr, m_info_ptr);
    m_ncols = png_get_image_width(m_png_ptr, m_info_ptr);
    m_nrows = png_get_image_height(m_png_ptr, m_info_ptr);
    m_color_type = png_get_color_type(m_png_ptr,m_info_ptr);
    m_bit_depth = png_get_bit_depth(m_png_ptr, m_info_ptr);
    png_read_update_info(m_png_ptr, m_info_ptr);

    /* read file */
    if (setjmp(png_jmpbuf(m_png_ptr))){
        fclose(fp);
        string m = "[read_png_file] Error during read_image\n";
        throw runtime_error(m);
    }


    m_row_buffers = new png_bytep[m_nrows];
    int bytes_per_row = png_get_rowbytes(m_png_ptr,m_info_ptr);
    for (int y=0; y<m_nrows; y++){
        m_row_buffers[y] = new png_byte[bytes_per_row];
    }

    png_read_image(m_png_ptr, m_row_buffers);

    fclose(fp); fp=NULL;
    m_valid=true;
}

png_reader::~png_reader(){
    for(int y=0; y<m_nrows; y++){
        delete [] m_row_buffers[y];
        m_row_buffers[y]=NULL;
    }
    delete [] m_row_buffers; m_row_buffers=NULL;
}

RGBColor png_reader::operator()(int row, int col){
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

    if (png_get_color_type(m_png_ptr, m_info_ptr)
            != PNG_COLOR_TYPE_RGB){
        throw runtime_error("PNG image is not RGB\n");
    }

    png_byte* row_buf = m_row_buffers[row];
    //Assumes RGB 3 bytes per pixel
    png_byte* pix_ptr = &row_buf[col*3];

    return RGBColor(pix_ptr[0],pix_ptr[1],pix_ptr[2]);

}

