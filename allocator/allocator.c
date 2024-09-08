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

    LOG(INFO,"Allocator has been initialized.");
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
                LOG_SYS(ERROR,"Failed to remove a free chunk from available chunks.");
                LOG(ERROR,ALLOC_ERR_MSG);
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
                LOG_SYS(ERROR,"Failed to insert the tailing chunk (after removing the required chunk) back to Free chunk list.");
                LOG(ERROR,ALLOC_ERR_MSG);
                return NULL;
            }
            }
            if(Allocated_Heap_Chunks.len==HEAP_CHUNK_CAP)
            {

                LOG_SYS(ERROR,"Allocated heap chunk lists capacity had been hit.");
                LOG(ERROR,ALLOC_ERR_MSG);
                return NULL;
            }
            HeapChunk * res_chunk=heap_chunk_insert(&Allocated_Heap_Chunks,&required_chunk);
            if (res_chunk)
            return res_chunk->start;
            else
            {
                LOG_SYS(ERROR,"Failed to insert the free chunk (which was taken from free chunk list) into Allocated Chunks");
                LOG(ERROR,ALLOC_ERR_MSG);
                return NULL;
            }        
            
        }
    }
    LOG_SYS(ERROR,"Failed to find any free chunk. Either all available memory has been used or heap has severly fragmented.");
    LOG(ERROR,ALLOC_ERR_MSG);
    return NULL;

}
bool heap_free(void * ptr)
{

    HeapChunk remove_res=heap_chunk_remove(&Allocated_Heap_Chunks,ptr,-1);
    if(remove_res.start==NULL)
    {
    LOG_SYS(ERROR,"Failed to remove the chunk from allocated chunk list.");
    LOG(ERROR,DEALLOC_ERR_MSG);
    return false;
    }
    if(Freed_Heap_Chunks.len==HEAP_CHUNK_CAP)
    {
        LOG_SYS(ERROR,"Free heap chunk lists capacity had been hit.");
        LOG(ERROR,DEALLOC_ERR_MSG);
        return false;
    }
    void * insert_res=heap_chunk_insert(&Freed_Heap_Chunks,&remove_res);
    if(insert_res==NULL)
    {
    LOG_SYS(ERROR,"Failed to insert the freed chunk into freed chunk list.");
    LOG(ERROR,DEALLOC_ERR_MSG);
    return false;
    }
    return true;
}