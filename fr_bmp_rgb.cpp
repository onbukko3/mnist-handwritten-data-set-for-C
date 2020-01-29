#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bmp.h"


int main()
{
    FILE *pf = fopen("data/RGB_24bits_palette_color.bmp", "rb");
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    
    char *image;
    int size;
    int height;
    int width;
    short bitperpixel;
    char *paddingset;
    int padding;

    
    if(pf!=NULL){


        fread(&bfh,sizeof(BITMAPFILEHEADER),1,pf);
        fread(&bih,sizeof(BITMAPINFOHEADER),1,pf);

        padding = bfh.bfOffBits -sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
        paddingset = (char *)malloc(padding);
        
        fread(paddingset,padding,1,pf );
        
        bitperpixel = bih.biBitCount;

        size = bih.biSizeImage;
        height = bih.biHeight;
        width = bih.biWidth;

        fseek(pf, bfh.bfOffBits,SEEK_SET);
        
        image = (char*)malloc(size);

        fread(image, size, 1, pf);
    
    }
    fclose(pf);

    FILE *pf_1 = fopen("data/RGB_24bits_palette_color_result.bmp","wb");

    int idx;
    int j;
    int idx_blue;
    int idx_green;
    int idx_red;
    char *buf = (char*)malloc(size);
    int period = width -1;

    if(pf_1!=NULL)
    {

        fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, pf_1);

        fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, pf_1);
        fwrite(paddingset, padding, 1, pf_1);

        j=0;    
        for(idx=0; idx<size; idx++)
        {
            idx_blue = 3*((height)+j*(period))-1;
            idx_green = 3*((height)+j*(period));
            idx_red = 3*((height)+j*(period))+1;

            if(idx==idx_blue)
            {
                buf[idx] = -1;
            }
            if(idx == idx_green)
            {
                buf[idx] = -1;
            }
            else if(idx == idx_red)
            {
                buf[idx] = 0;
                if(j== height-20)
                {
                    j;
                }
                else
                {
                    j++;
                    
                } 
                
            }
            else
            {
                buf[idx] = image[idx];
            }

        }
            printf("%d\n", height);
            printf("%d",j);
        // }
        
        fwrite(buf,size,1,pf_1);
    }

    fclose(pf_1);




    return 0;

}
