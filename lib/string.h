#ifndef STRING_H
#define STRING_H
#include <stdint.h>

int strcmp(const char* s1, const char* s2);
int int_to_ascii(int n, char* buf);
void *memset(void *dest, int val, uint32_t len);

#endif