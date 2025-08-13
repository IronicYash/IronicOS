#ifndef IRT_PORTS_H
#define IRT_PORTS_H

#include <stdint.h>

/*
 * Port I/O for x86
 * All functions are marked static inline so that GCC inlines them into callers,
 * avoiding separate object files and linker issues.
 * "a" forces use of the AL/AX/EAX register family correctly sized for the operand.
 * "Nd" allows an immediate port number if constant, or DX register otherwise.
 */

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

#endif /* IRT_PORTS_H */