#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <jpeglib.h>
#include <setjmp.h>
#include "bmp.h"


struct jpeg_decompress_struct cinfo;

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

// JSAMPARRAY buffer;
unsigned char *buffer_bmp;

BITMAPFILEHEADER _bfh;
BITMAPINFOHEADER _bih;

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{

    my_error_ptr myerr = (my_error_ptr) cinfo->err;

    (*cinfo->err->output_message) (cinfo);

    longjmp(myerr->setjmp_buffer,1);
}


GLOBAL(int)
read_jpeg_file (char *filename)
{

    struct my_error_mgr jerr;
    FILE *srcfile;
    int row_stride;

    if((srcfile = fopen(filename,"rb")) == NULL)
    {
        fprintf(stderr, "cannot open %s\n", filename);
        return 0;
    }

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;

    if(setjmp(jerr.setjmp_buffer)){
        jpeg_destroy_decompress(&cinfo);
        fclose(srcfile);
        return 0;
    }

    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, srcfile);

    (void) jpeg_read_header(&cinfo, TRUE);

    (void) jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;

    // buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

    buffer_bmp = (unsigned char*)malloc(row_stride * cinfo.output_height);

    while (cinfo.output_scanline < cinfo.output_height)
    {   
        unsigned char* buffer[1];

        buffer[0] = buffer_bmp + (cinfo.output_scanline) * row_stride;
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    }

    (void) jpeg_finish_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);

    fclose(srcfile);

    return 1;
    
}

METHODDEF(void)
convert_jpeg_to_bmp()
{
    //bmp file header
    _bfh.bfType = ((unsigned int)('M' << 8) | 'B');
    _bfh.bfReserved1 = 0;
    _bfh.bfReserved2 = 0;
    _bfh.bfSize = 122;
    _bfh.bfOffBits = 108;
    
    //bmp info header
    _bih.biBitCount = cinfo.output_components*8;
    _bih.biWidth = cinfo.output_width;
    _bih.biHeight = cinfo.output_height;
    _bih.biSizeImage = _bih.biWidth * _bih.biHeight;

}

GLOBAL(int)
write_bmp_file(char *filename)
{
    FILE *tgtFile;

    if((tgtFile = fopen(filename,"wb")) == NULL)
    {
        fprintf(stderr, "cannot write %s\n", filename);
        return 0;
    }

    fwrite(&_bfh, 1, sizeof(BITMAPFILEHEADER), tgtFile);
    fwrite(&_bih, 1, sizeof(BITMAPINFOHEADER), tgtFile);
    fwrite(buffer_bmp, 1, cinfo.output_width*cinfo.output_height*cinfo.output_components, tgtFile);

    fclose(tgtFile);

    return 1;
}

int main()
{
    //int argc, char* argv[]
    // if(argc<3)
    // {
    //     printf("usage : {app Name} {input JPEG file path} {output BMP file path}\n");
	// 	return 0;
    // }
    
    read_jpeg_file("data/jpeg/4k-wallpaper-automobile-automotive-branding-1149137.jpg");
    
    convert_jpeg_to_bmp();

    write_bmp_file("data/jpeg/4k-wallpaper-automobile-automotive-branding-1149137.bmp");

    return 0;

}