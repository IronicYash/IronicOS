#include "string.h"

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
int int_to_ascii(int n, char* buf) {
    int i = 0;
    if (n == 0) {
        buf[i++] = '0';
    } else {
        while (n > 0) {
            buf[i++] = '0' + (n % 10);
            n /= 10;
        }
    }
    buf[i] = '\0';

    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char temp = buf[j];
        buf[j] = buf[i - j - 1];
        buf[i - j - 1] = temp;
    }
    return i; // Return the length of the string
}

void *memset(void *dest, int value, uint32_t len) {
    unsigned char *ptr = dest;
    while (len-- > 0) *ptr++ = (unsigned char)value;
    return dest;
}