#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
void* memset(void* dest, int val, size_t len);
void* memcpy(void* dest, const void* src, size_t len);
int memcmp(const void* ptr1, const void* ptr2, size_t n);

#endif