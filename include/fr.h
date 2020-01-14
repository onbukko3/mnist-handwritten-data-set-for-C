#ifndef _FR_H_
#define _FR_H_

typedef enum
{
    FALSE,
    TRUE
} BOOL;

int getStringLength(const char *str);
BOOL copyString(char *dst, const char *src);

int _process(int argc, char *argv[]);

#endif