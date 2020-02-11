#include "jpeg-build/include/jpeglib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    struct jpeg_decompress_struct cinfo;

    FILE *pf = fopen("./data/jpeg/4k-wallpaper-automobile-automotive-branding-1149137.jpg","rb");
    JSAMPARRAY buffer;
    int row_stride;

    if(pf == NULL)
    {
        fprintf(stderr, "can't open it! \n");
        return 0;
    }

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, pf);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;

    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

    while (cinfo.output_scanline < cinfo.output_height)
    {
        jpeg_read_scanlines(&cinfo, buffer, 1);

        put_scanline_someplace(buffer[0], row_stride);

    }

    jpeg_finish_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);

    fclose(pf);

    return 1;

}