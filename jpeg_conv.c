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
unsigned char* buffer[1];


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
    buffer_bmp = (unsigned char*)malloc(row_stride*cinfo.output_height);
    int i  = 0;
    int j = 0;
    while (cinfo.output_scanline < cinfo.output_height)
    {   

        buffer[0] = buffer_bmp + (cinfo.output_scanline) * row_stride;
        (void) jpeg_read_scanlines(&cinfo, &buffer[0], 1);
        // memcpy(buffer_bmp, *buffer, sizeof(buffer));   
        
    }
    // printf("the number of size: %ld\n", sizeof(buffer_bmp));
    (void) jpeg_finish_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);

    fclose(srcfile);
    
    // free(buffer);
    return 1;
    
}

METHODDEF(void)
convert_jpeg_to_bmp()
{
    //bmp file header
    _bfh.bfType = ((unsigned int)('M' << 8) | 'B');
    _bfh.bfReserved1 = 0;
    _bfh.bfReserved2 = 0;
    _bfh.bfSize = cinfo.output_height*cinfo.output_components + 54;
    _bfh.bfOffBits = 108;
    
    //bmp info header
    _bih.biSize = 40;
    _bih.biBitCount = cinfo.output_components*8;
    _bih.biWidth = cinfo.output_width;
    _bih.biHeight = cinfo.output_height;
    _bih.biSizeImage = _bih.biWidth * _bih.biHeight*cinfo.output_components;

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
   
    int i;
    int j;
    int padding=0;
    int WIDTH;
    int count=0;

    if(tgtFile!=NULL)
    {

        fwrite(&_bfh, 1, sizeof(BITMAPFILEHEADER), tgtFile);
        fwrite(&_bih, 1, sizeof(BITMAPINFOHEADER), tgtFile);
        // traverse the picture 
        long size = _bih.biSizeImage;
        int count = _bih.biBitCount;
        char temp;
        padding = ((PIXEL_ALIGN - ((_bih.biWidth * _bih.biBitCount / 8) % PIXEL_ALIGN)) % PIXEL_ALIGN);
        int pixelsize = getPixelSize(_bih.biBitCount);
        WIDTH = _bih.biWidth*pixelsize + padding;

        unsigned char *image = (unsigned char *)malloc(size);


        for(int i =0; i<size/2; ++i )
        {
            temp = buffer_bmp[i];
            buffer_bmp[i] = buffer_bmp[size -1 -i];
            buffer_bmp[size -1 -i] = temp;
        }
        
        memcpy(image, buffer_bmp, size);

        int HEIGHT = _bih.biHeight -1;
        for(i=HEIGHT;i>=0 ; i--)
        {
            for(j=0;j<(_bih.biWidth);j++)
            {
                // if(j<_bih.biWidth/2)
                // {
                //     temp = buffer_bmp[(i * WIDTH) + (j*pixelsize) ];
                //     buffer_bmp[(i * WIDTH) + (j*pixelsize) ] = buffer_bmp[((i+1) * WIDTH)-1-(j*pixelsize)];
                //     buffer_bmp[((i+1) * WIDTH) -1-(j*pixelsize)] = temp;

                // }

                pRGBTRIPLE pRGB = (pRGBTRIPLE)&buffer_bmp[(i * WIDTH) + (j * pixelsize)];
                pRGB -> rgbtBlue = image[(i * WIDTH) + ((_bih.biWidth - j) * pixelsize)];
                pRGB -> rgbtGreen = image[(i * WIDTH) + ((_bih.biWidth - j) * pixelsize)+1];
                pRGB -> rgbtRed = image[(i * WIDTH) + ((_bih.biWidth - j) * pixelsize)+2];  

            }
            
        }

        fwrite(buffer_bmp,1,size,tgtFile);
    }

    free(buffer_bmp);

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
    
    read_jpeg_file("data/jpeg/view-of-elephant-in-water-247431.jpg");
    
    convert_jpeg_to_bmp();

    write_bmp_file("data/jpeg/view-of-elephant-in-water-247431.bmp");

    return 0;

}