#ifndef KERNEL_H
#define KERNEL_H

// Kernel function declarations
void kernel_init(void);
void kernel_start(void);

// Data structure definitions
typedef struct {
    int id;
    char name[32];
} process_t;

// Constants
#define MAX_PROCESSES 256

#endif // KERNEL_H