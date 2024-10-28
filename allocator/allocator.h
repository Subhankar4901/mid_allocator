#ifndef ALLOCATOR_H
#define ALLOCATOR_H
#include<stdbool.h>
#include<stddef.h>
void init_allocator(size_t memory_capacity_in_bytes);
void * mid_alloc(size_t size);
bool mid_free(void * ptr);
void mid_collect();
void allocated_mem_dump();
void free_mem_dump();
#endif