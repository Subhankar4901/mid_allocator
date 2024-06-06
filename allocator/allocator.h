#ifndef ALLOCATOR_H
#define ALLOCATOR_H
#include<stdio.h>
#include "../utils/utils.h"
char Heap[HEAP_SIZE];
HeapChunk_List Allocated_Heap_Chunks;
HeapChunk_List Freed_Heap_Chunks;
void init_allocator(void);
void * heap_alloc(size_t size);
bool heap_free(void * ptr);
#endif