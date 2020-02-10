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
    int startline;
    int endline;

    if(pf_1!=NULL)
    {

        fwrite(&_bfh, 1, sizeof(BITMAPFILEHEADER), pf_1);
        fwrite(&_bih, 1, sizeof(BITMAPINFOHEADER), pf_1);
        fwrite(bif_plus_set, 1, _bih.biSize-sizeof(BITMAPINFOHEADER),pf_1);
        padding = ((PIXEL_ALIGN - ((width * bitcount / 8) % PIXEL_ALIGN)) % PIXEL_ALIGN);
        pixelsize = getPixelSize(bitcount);
        WIDTH = width*pixelsize + padding;

        startline = height-20;
        endline = (startline)/2;
        int j_start = 20;
        int j_end = (width)/2;

        for(i=startline;i>=endline ; i--)
        {
            if(i==startline|| i ==endline){

                for(j=j_start;j<j_end+1;j++)
                {
                    pRGBTRIPLE pRGB = (pRGBTRIPLE)&image[(i * WIDTH) + (j * pixelsize)];
                    pRGB->rgbtRed = 0;
                    pRGB->rgbtGreen = 0;
                    pRGB->rgbtBlue = 255;
                
                }
            }
            else if(i>endline && i <startline)
            {
                pRGBTRIPLE pRGB_start = (pRGBTRIPLE)&image[(i * WIDTH) + (j_start * pixelsize)];
                pRGB_start->rgbtRed = 0;
                pRGB_start->rgbtGreen = 0;
                pRGB_start->rgbtBlue = 255;

                pRGBTRIPLE pRGB_end = (pRGBTRIPLE)&image[(i * WIDTH) + (j_end * pixelsize)];
                pRGB_end->rgbtRed = 0;
                pRGB_end->rgbtGreen = 0;
                pRGB_end->rgbtBlue = 255;
                
            }
            
        }
                   
        
        fwrite(image,1,_bih.biSizeImage,pf_1);
    }

    free(image);

    fclose(pf_1);

    return 0;

}
