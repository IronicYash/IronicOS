#include "memory.h"
#include <stdint.h>
#include "../cpu/ports.h"

#define HEAP_START 0x10000
#define HEAP_SIZE  0x10000  // 64 KB heap size

typedef struct FreeBlock {
    size_t size;
    struct FreeBlock* next;
} FreeBlock;

static uint32_t heap_top = HEAP_START;
static FreeBlock* free_list = NULL;

void* kmalloc(size_t size) {
    // Round size up to multiple of 4 for alignment
    if (size % 4 != 0) size += 4 - (size % 4);

    FreeBlock** curr = &free_list;
    while (*curr) {
        if ((*curr)->size >= size) {
            void* result = (void*)(*curr);
            *curr = (*curr)->next;
            return result;
        }
        curr = &((*curr)->next);
    }

    // No free block, use heap
    void* ptr = (void*)heap_top;
    heap_top += size;
    return ptr;
}

void kfree(void* ptr) {
    FreeBlock* block = (FreeBlock*)ptr;
    block->size = 0;  // unknown size, simplified
    block->next = free_list;
    free_list = block;
}

// kernel/memory.c
void init_memory() {
    // Your memory setup logic here
    free_list = (FreeBlock*)HEAP_START;
    free_list->size = HEAP_SIZE - sizeof(FreeBlock);
    free_list->next = NULL;
    heap_top = HEAP_START + sizeof(FreeBlock);
    // Initialize the heap with a single free block
    kfree((void*)HEAP_START);
}
