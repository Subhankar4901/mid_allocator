#include<stdio.h>
#include "allocator.h"
#include "../utils/utils.h"
void init_allocator()
{
    for(size_t i=0;i<HEAP_SIZE;i++)
    {
        Heap[i]='\0';
    }
    Allocated_Heap_Chunks.len=0;
    HeapChunk null_chunk={
        .start=NULL,
        .size=0
    };
    for(size_t i=0;i<HEAP_CHUNK_CAP;i++)
    {
        Allocated_Heap_Chunks.Heap_Chunks[i]=null_chunk;
        Freed_Heap_Chunks.Heap_Chunks[i]=null_chunk;
    }
    HeapChunk free_chunk={
        .start=Heap,
        .size=sizeof(Heap)
    };
        Freed_Heap_Chunks.Heap_Chunks[0]=free_chunk;
        Freed_Heap_Chunks.len=1;
}
void * heap_alloc(size_t size)
{
    if(size<=0)
    return NULL;
    merge_freed_chunks(&Freed_Heap_Chunks);
    for(size_t i=0;i<Freed_Heap_Chunks.len;i++)
    {
        HeapChunk cur_chunk=Freed_Heap_Chunks.Heap_Chunks[i];
        if(cur_chunk.size>=size)
        {
            HeapChunk remove_res= heap_chunk_remove(&Freed_Heap_Chunks,cur_chunk.start,-1);
            if(remove_res.start==NULL && remove_res.size==0)
            {
                printf("Failed to remove the available chunk.");
                POSITION;
                return NULL;
            }
            HeapChunk required_chunk={
                .start=cur_chunk.start,
                .size=size
            };
            if(cur_chunk.size>size)
            {
            HeapChunk tail_chunk={
                .start=cur_chunk.start+size,
                .size=cur_chunk.size-size
            };
            void * _tail_insertion_res_= heap_chunk_insert(&Freed_Heap_Chunks,&tail_chunk);
            if(_tail_insertion_res_==NULL)
            {
                POSITION;
                printf("Failed to insert the tailed chunk.");
                return NULL;
            }
            }
            HeapChunk * res_chunk=heap_chunk_insert(&Allocated_Heap_Chunks,&required_chunk);
            if (res_chunk)
            return res_chunk->start;
            else
            {
                POSITION;
                printf("Failed to insert into Allocated Chunks");
                return NULL;
            }        
            
        }
    }
    printf("Couldn't find any free chunks.");
    POSITION;
    return NULL;

}
bool heap_free(void * ptr)
{

    HeapChunk remove_res=heap_chunk_remove(&Allocated_Heap_Chunks,ptr,-1);
    if(remove_res.start==NULL)
    {
    printf("Failed to remove from Allocated chunks.\n");
    POSITION;
    return false;
    }
    if(Freed_Heap_Chunks.len==HEAP_CHUNK_CAP)
    {
        printf("Reached Freed chunk capacity.\n");
        POSITION;
        return false;
    }
    void * insert_res=heap_chunk_insert(&Freed_Heap_Chunks,&remove_res);
    if(insert_res==NULL)
    {
    printf("Insertion into Freed chunks failed.\n");
    POSITION;
    return false;
    }
    return true;
}