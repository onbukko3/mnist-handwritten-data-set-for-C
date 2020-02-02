#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bmp.h"


int main()
{
    FILE *pf = fopen("data/sample.bmp", "rb");

    BITMAPFILEHEADER _bfh;
    BITMAPINFOHEADER _bih;
    
    unsigned char *image;
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

        image = (unsigned char*)malloc(_bih.biSizeImage);
        fread(image, _bih.biSizeImage, 1, pf);
    
    }
    fclose(pf);

    //write the new file

    FILE *pf_1 = fopen("data/sample_result.bmp","wb");
    int i;
    int j;
    int padding=0;
    int WIDTH;
    int pixelsize=0;

    if(pf_1!=NULL)
    {

        fwrite(&_bfh, 1, sizeof(BITMAPFILEHEADER), pf_1);
        fwrite(&_bih, 1, sizeof(BITMAPINFOHEADER), pf_1);
        fwrite(bif_plus_set, 1, _bih.biSize-sizeof(BITMAPINFOHEADER),pf_1);
        padding = ((PIXEL_ALIGN - ((width * bitcount / 8) % PIXEL_ALIGN)) % PIXEL_ALIGN);
        pixelsize = getPixelSize(bitcount);
        WIDTH = width*pixelsize + padding;

        for(i=(height-1);i>=0 ; i--)
        {
            for(j=0;j<width;j++)
            {
                if(j== (height-1-i))
                {
                    pRGBTRIPLE pRGB = (pRGBTRIPLE)&image[(i * WIDTH) + (j * pixelsize)];
                    pRGB->rgbtRed = 255;
                    pRGB->rgbtGreen = 0;
                    pRGB->rgbtBlue = 0;

                    
                }
                else if(j==i)
                {
                    pRGBTRIPLE pRGB_1 = (pRGBTRIPLE)&image[(i * WIDTH) + (j * pixelsize)];
                    pRGB_1->rgbtRed = 255;
                    pRGB_1->rgbtGreen = 0;
                    pRGB_1->rgbtBlue = 0;
                    
                }


            }
            
        }
                   
        
        fwrite(image,1,_bih.biSizeImage,pf_1);
    }

    free(image);

    fclose(pf_1);

    return 0;

}
