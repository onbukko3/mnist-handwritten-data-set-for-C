#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bmp.h"

static unsigned char *pImagebuf;

unsigned char *getbuff(void)
{
    return pImagebuf;
}

int main()
{
    FILE *pf = fopen("data/sample.bmp", "rb");
    pBITMAPFILEHEADER bfh;
    pBITMAPINFOHEADER bih;
    BITMAPFILEHEADER _bfh;
    BITMAPINFOHEADER _bih;
    
    char *image;
    int size;
    int height;
    int width;
    short bitperpixel;
    unsigned char *bif_plus_set;
    int bif_plus;
    int bitcount;
    
    if(pf!=NULL){


        fread(&_bfh,sizeof(BITMAPFILEHEADER),1,pf);
        fread(&_bih,sizeof(BITMAPINFOHEADER),1,pf);

        width = _bih.biWidth;
        height = _bih.biHeight;
        bitcount = _bih.biBitCount;

        bif_plus_set = (unsigned char*)malloc(_bih.biSize-sizeof(BITMAPINFOHEADER));
        fread(bif_plus_set, _bih.biSize-sizeof(BITMAPINFOHEADER),1,pf);
    
    }
    fclose(pf);

    FILE *pf_1 = fopen("data/sample_result.bmp","wb");
    int i;
    int j=0;
    int padding=0;
    pImage buf;
    int period = width -1;
    int WIDTH;
    int pixelsize=0;

    if(pf_1!=NULL)
    {

        fwrite(&_bfh, 1, sizeof(BITMAPFILEHEADER), pf_1);

        fwrite(&_bih, 1, sizeof(BITMAPINFOHEADER), pf_1);
        fwrite(bif_plus_set, 1, _bih.biSize-sizeof(BITMAPINFOHEADER),pf_1);
        buf = getbuff();
        padding = ((PIXEL_ALIGN - ((width * bitcount / 8) % PIXEL_ALIGN)) % PIXEL_ALIGN);
        pixelsize = getPixelSize(bitcount);
        WIDTH = width*pixelsize + padding;

        for(i=(height-1);i>=0 ; i--)
        {
            for(j;j<width;j++)
            {
                if(j== (height-1-i))
                {
                    pRGBTRIPLE pRGB = (pRGBTRIPLE)&buf[(i * WIDTH) + (j * pixelsize)];
                    pRGB->rgbtRed = 255;
                    pRGB->rgbtGreen = 0;
                    pRGB->rgbtBlue = 0;
                }

            }
            
        }
           

        printf("%d",j);
        
        
        fwrite(buf,1,_bih.biSizeImage,pf_1);
    }
    free(buf);

    fclose(pf_1);





    return 0;

}
