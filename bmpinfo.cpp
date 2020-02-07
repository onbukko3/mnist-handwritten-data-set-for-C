#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/bmpinfo.h"

#define errorReturn(FILE_POINTER) \
do{ \
printf("*error occured in %s:%d\n", __FILE__ , __LINE__); \
fclose(FILE_POINTER); \
return FALSE; \
} while(0);

static BMPFileHeader _bmpFileHeader;
static BMPInfoHeader _bmpInfoHeader;
static unsigned char *pImgBuf;
static unsigned char *pInfoHeader;

// 24bit BMP file case
BOOL openBMPFile(char *path)
{
    BOOL ret = TRUE;
    FILE *fp;
    fp = fopen(path, "rb");

    if(fp == NULL)
    {
        ret = FALSE;
    }
    else
    {
        int padding = 0;
		int WIDTH = 0;
		
        if(fread(&_bmpFileHeader, sizeof(BMPFileHeader), 1, fp) < 1)
            errorReturn(fp);

        if(_bmpFileHeader.bfType != ((DWORD)('M' << 8) | 'B')) // "BM"
            errorReturn(fp);

        if(fread(&_bmpInfoHeader, sizeof(BMPInfoHeader), 1, fp) < 1)
            errorReturn(fp);

        if(_bmpInfoHeader.biSize > sizeof(BMPInfoHeader))
        {
            pInfoHeader = (unsigned char*)malloc(_bmpInfoHeader.biSize - sizeof(BMPInfoHeader));
            fread(pInfoHeader, _bmpInfoHeader.biSize - sizeof(BMPInfoHeader), 1, fp);
        }

        if(_bmpInfoHeader.biBitCount != 24)
            errorReturn(fp);

        pImgBuf = (unsigned char*)malloc(_bmpInfoHeader.biSizeImage);
        //fseek(fp, _bmpFileHeader.bfOffBits, SEEK_SET);
        if(fread(pImgBuf, sizeof(unsigned char), _bmpInfoHeader.biSizeImage, fp) < 1)
            errorReturn(fp);

        fclose(fp);
    }
    return ret;
}

void closeBMP(void)
{
    free(pImgBuf);
}

BOOL writeBMPFile(char *path)
{
    FILE *fp;
    BOOL ret = FALSE;
    fp = fopen(path, "wb");

    if(fp)
    {
        if(fwrite(&_bmpFileHeader, 1, sizeof(BMPFileHeader), fp) < 1)
            errorReturn(fp);
        if(fwrite(&_bmpInfoHeader, 1, sizeof(BMPInfoHeader), fp) < 1)
            errorReturn(fp);
        
        if((_bmpInfoHeader.biSize - sizeof(BMPInfoHeader)) > 0)
        {
            if(fwrite(pInfoHeader, 1, _bmpInfoHeader.biSize - sizeof(BMPInfoHeader), fp) < 1)
                errorReturn(fp);
        }

        if(fwrite(pImgBuf, 1, _bmpInfoHeader.biSizeImage, fp) < 1)
            errorReturn(fp);
        //free(pInfoHeader);
        fclose(fp);
    }
    else
    {
        ret = FALSE;
    }
    return ret;
}

pBMPFileHeader getBMPFileHeader(void)
{
    return &_bmpFileHeader;
}

pBMPInfoHeader getBMPInfoHeader(void)
{
    return &_bmpInfoHeader;
}

unsigned char *getBMPBuffer(void)
{
    return pImgBuf;
}

void _bmpProcess(int argc, char* argv[])
{
	pBMPFileHeader pfile;
	pBMPInfoHeader pinfo;
	pImg pimage;
	int widthCnt = 0;
	int hightCnt = 0;

	if(argc < 3)
	{
		printf("usage : {app Name} {input BMP file path} {output BMP file path}\n");
		return;
	}

	if(openBMPFile(argv[1]))
	{
		int padding = 0;
		int WIDTH = 0;
		int pixelSize = 0;
		printf("open BMP file : %s\n", argv[1]);
		pfile = getBMPFileHeader();
		pinfo = getBMPInfoHeader();
		pimage = getBMPBuffer();
		padding = getPadding(pinfo->biWidth , pinfo->biBitCount); // 8: 1byte = 8bit, align with 4byte
		pixelSize = getPixelSize(pinfo->biBitCount);
		WIDTH = pinfo->biWidth * pixelSize + padding;
		for(hightCnt = pinfo->biHeight - 1; hightCnt >= 0 ; hightCnt--)
		{
			for(widthCnt = 0; widthCnt < pinfo->biWidth; widthCnt++)
			{
				if((pinfo->biHeight - 1 - hightCnt) == widthCnt)
				{
					pRGBTRI pRGB = (pRGBTRI)&pimage[(hightCnt * WIDTH) + (widthCnt * pixelSize)];
					pRGB->r = 255;
					pRGB->g = 0;
					pRGB->b = 0;
				}
			}
		}

		writeBMPFile(argv[2]);
	}
	else
	{
		printf("fail to open BMP file : %s\n", argv[1]);
	}
}

void _mnistProc(int argc, char* argv[])
{
	pBMPFileHeader pfile;
	pBMPInfoHeader pinfo;
	pImg pimage;
	int widthCnt = 0;
	int hightCnt = 0;

	if(argc < 3)
	{
		printf("usage : {app Name} {input BMP file path} {output BMP file path}\n");
		return;
	}

	if(openBMPFile(argv[1]))
	{
		int padding = 0;
		int WIDTH = 0;
		int pixelSize = 0;
        char name[10];
        int count = 0;
        int sample = 0;
        int mn = 0;
        FILE *pmnist;
        unsigned char buf;

        pmnist = fopen(argv[2], "rb");
        //fseek(pmnist, sizeof(int), SEEK_SET);
        fread(&mn, 1, 4, pmnist);
        mn = be32toh(mn);
        fread(&sample, 1, 4, pmnist);
        sample = be32toh(sample);
        fseek(pmnist, sizeof(int)* 2, SEEK_CUR);

		printf("open BMP file : %s\n", argv[1]);
		pfile = getBMPFileHeader();
		pinfo = getBMPInfoHeader();
		pimage = getBMPBuffer();
		padding = getPadding(pinfo->biWidth , pinfo->biBitCount); // 8: 1byte = 8bit, align with 4byte
		pixelSize = getPixelSize(pinfo->biBitCount);
		WIDTH = pinfo->biWidth * pixelSize + padding;
		
        for(count = 0; count < sample; count++)
        {
            sprintf(name, "%s_%d.bmp", argv[2], count);
            for(hightCnt = pinfo->biHeight - 1; hightCnt >= 0 ; hightCnt--)
            {
                for(widthCnt = 0; widthCnt < pinfo->biWidth; widthCnt++)
                {
                    pRGBTRI pRGB = (pRGBTRI)&pimage[(hightCnt * WIDTH) + (widthCnt * pixelSize)];
                    fread(&buf, sizeof(unsigned char), 1, pmnist);
                    pRGB->r = buf;
                    pRGB->g = buf;
                    pRGB->b = buf;
                }
            }
            writeBMPFile(name);
        }
        fclose(pmnist);
        free(pInfoHeader);
	}
	else
	{
		printf("fail to open BMP file : %s\n", argv[1]);
	}
}