#include<stdio.h>
#include "../utils/utils.h"
void heap_chunk_dump(HeapChunk_List*list){
    for(size_t i=0;i<list->len;i++)
    {
        printf("Chunk of Size : %zu is at location : %p\n",list->Heap_Chunks[i].size,list->Heap_Chunks[i].start);
    }
}