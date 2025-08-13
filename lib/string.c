#include "string.h"

size_t strlen(const char *s) {
    const char *p = s;
    while (*p) ++p;
    return (size_t)(p - s);
}

void memset(void *dest, int val, size_t len) {
    unsigned char *d = (unsigned char*)dest;
    while (len--) *d++ = (unsigned char)val;
}

void memcpy(void *dest, const void *src, size_t len) {
    unsigned char *d = (unsigned char*)dest;
    const unsigned char *s = (const unsigned char*)src;
    while (len--) *d++ = *s++;
}

int strcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) { ++a; ++b; }
    return (unsigned char)*a - (unsigned char)*b;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    if (n == 0) {
        return 0;
    }

    while (n-- != 0) {
        unsigned char c1 = (unsigned char)*s1++;
        unsigned char c2 = (unsigned char)*s2++;

        if (c1 != c2) {
            return c1 - c2;
        }

        if (c1 == '\0') {
            break;
        }
    }

    return 0;
}