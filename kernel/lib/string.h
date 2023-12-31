#ifndef _VIZ_LIB_STRING_H
#define _VIZ_LIB_STRING_H

#include <stddef.h>

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int strncmp(const char *s1, const char *s2, size_t n);

#endif