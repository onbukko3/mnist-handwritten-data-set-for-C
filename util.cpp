#include <stdio.h>
#include "include/util.h"

BOOL copyString(char *dst, const char *src)
{
    int srcLength = getStringLength(src);
    int i = 0;
    int dstLength = 0;
    BOOL retVal = TRUE;

    for(i = 0; i < srcLength; i++)
    {
        dst[i] = src[i];
    }

    dstLength = getStringLength(dst);
    if(dstLength != srcLength)
        retVal = FALSE;

    return retVal;
}

int getStringLength(const char *str)
{
    int i = 0;
    int length = 0;

    while(str[i++] != '\0')
    {
        if(i > FILENAME_MAX)
            break;
        length++;
    }

    return length;
}

int getFilelineCount(char *filename)
{
    FILE *pf;
    int count = 0;
    pf = fopen(filename, "r");
    if(pf != NULL)
    {
        BOOL isEnd = FALSE;
        while(1)
        {
            switch (getc(pf))
            {
            case '\n':
                count++;
                break;
            case EOF:
                isEnd = TRUE;
                break;
            default:
                break;
            }

            if(isEnd)
                break;
        }
        fclose(pf);
    }

    return count;
}