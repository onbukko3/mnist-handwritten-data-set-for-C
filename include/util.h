#ifndef _UTIL_H_
#define _UTIL_H_
#include "userdef.h"

int getStringLength(const char *str);
BOOL copyString(char *dst, const char *src);
int getFilelineCount(char *filename);

#endif