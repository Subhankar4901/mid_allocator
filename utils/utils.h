#ifndef UTILS_H
#define UTILS_H
#include<stdbool.h>
#define HEAP_SIZE 640000 //640KB;
#define HEAP_CHUNK_CAP 4000 
#define POSITION \
            do{\
            printf("File : %s\n",__FILE__);\
            printf("Function : %s\n",__func__);\
            printf("Line : %d\n",__LINE__);\
            }while(0)
typedef struct
{
    void* start;
    size_t size;
}HeapChunk;
typedef struct
{
    HeapChunk Heap_Chunks[HEAP_CHUNK_CAP];
    size_t len;
}HeapChunk_List;
void heap_chunk_dump(HeapChunk_List*list);
int heap_chunk_query(HeapChunk_List* list,void*ptr);
void* heap_chunk_insert(HeapChunk_List* list,HeapChunk*ptr);
HeapChunk heap_chunk_remove(HeapChunk_List* list,void*ptr,int index);
void merge_freed_chunks(HeapChunk_List * list);

#endif