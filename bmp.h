
#pragma pack(push,1)

typedef struct _BITMAPFILEHEADER
{
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;

}BITMAPFILEHEADER;

typedef struct _BITMAPINFOHEADER
{
    unsigned int biSize; //The number of bytes required
    unsigned int biWidth;
    unsigned int biHeight;
    unsigned short biPlanes; // The number of planes of the target device / must be set to 1
    unsigned short biBitCount; // The number of bits per pixel. (determines the number of bits that define each pixel and max number color)
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXpelsPerMeter; // x resolution(in pixels per meter) of the target device
    int biYpelsPerMeter;
    unsigned int biClrUsed; // color used
    unsigned int biClrImport; // the number of color indexes that are required for displaying

} BITMAPINFOHEADER;

typedef struct _RGBTRIPLE
{
    unsigned char rgbtBlue;
    unsigned char rgbtGreen;
    unsigned char rgbtRed;

} RGBTRIPLE;

#pragma pack(pop)