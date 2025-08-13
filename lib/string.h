#ifndef IRT_STRING_H
#define IRT_STRING_H

#include <stddef.h>

size_t strlen(const char *s);
void memset(void *dest, int val, size_t len);
void memcpy(void *dest, const void *src, size_t len);
int strcmp(const char *a, const char *b);
int strncmp(const char *s1, const char *s2, size_t n);

#endif /* IRT_STRING_H */