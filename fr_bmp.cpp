#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bmp.h"


int main()
{
    FILE *pf = fopen("data/sample.bmp", "rb");
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    
    char *image;
    int size;
    int height;
    int width;
    short bitperpixel;

    
    if(pf!=NULL){


        fread(&bfh,sizeof(BITMAPFILEHEADER),1,pf);
        fread(&bih,sizeof(BITMAPINFOHEADER),1,pf);

        bitperpixel = bih.biBitCount;

        size = bih.biSizeImage;
        height = bih.biHeight;
        width = bih.biWidth;

        fseek(pf, bfh.bfOffBits,SEEK_SET);
        
        image = (char*)malloc(size);

        fread(image, size, 1, pf);
    
    }
    fclose(pf);

    FILE *pf_1 = fopen("data/sample_result.bmp","wb");

    int idx;
    int j;

    char *buf = (char*)malloc(size);
    int period = width -1;
    int padding = bfh.bfOffBits -sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);

    if(pf_1!=NULL)
    {

        fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, pf_1);

        fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, pf_1);

        j=0;
        for(idx=0; idx<size; idx++)
        {
            // idx = 0;
            // for(j=0;j<height;j++)
            // {
                if(idx == 3*((height)+j*(period)))
                {
                    if(image[idx] == -1)
                    {
                        buf[idx] = 0;
                    }
                }
// how can i manage the magic number 60 bytes for 20 pixels?

                else if(idx == 3*((height+20)+j*(period)))
                {
                    if(image[idx] == -1)
                    {
                        buf[idx] = 0;
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
