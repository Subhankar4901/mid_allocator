#include<stdio.h>
#include "utils/utils.h"
char Heap[HEAP_SIZE]={0};
size_t cur_heap_size=0;
HeapChunk_List Allocated_Heap_Chunks={
    .Heap_Chunks={0},
    .len=0
};
HeapChunk_List Freed_Heap_Chunks={
    .Heap_Chunks={0},
    .len=0
};
void * heap_alloc(size_t size)
{
    if (size<=0)
    {
    printf("Size is less than or equal to zero.\n");
    return NULL;
    }
    if(cur_heap_size+size>HEAP_SIZE)
    {    // heap_chunk_dump(Heap_Chunks);
    printf("Heap reached it's capacity.\n");
    return NULL;
    }
    if(Allocated_Heap_Chunks.len==HEAP_CHUNK_CAP)
    {
        printf("Reached Chunk allocation capacity.\n");
        return NULL;
    }
    HeapChunk cur_chunk={
        .start=Heap+cur_heap_size,
        .size=size
    };
    void * res=heap_chunk_insert(&Allocated_Heap_Chunks,&cur_chunk);
    if(res==NULL)
        {
        printf("Chunk Insertion failed.\n");
        return NULL;
        }
    cur_heap_size+=size;
    return cur_chunk.start;
}
bool heap_free(void * ptr)
{

    HeapChunk remove_res=heap_chunk_remove(&Allocated_Heap_Chunks,ptr);
    if(remove_res.start==NULL)
    {
    printf("Failed to remove from Allocated chunks.\n");
    return false;
    }
    if(Freed_Heap_Chunks.len==HEAP_CHUNK_CAP)
    {
        printf("Reached Freed chunk capacity.\n");
        return false;
    }
    void * insert_res=heap_chunk_insert(&Freed_Heap_Chunks,&remove_res);
    if(insert_res==NULL)
    {
    printf("Insertion into Freed chunks failed.\n");
    return false;
    }
    cur_heap_size-=remove_res.size;
    return true;
}
int main()
{
    int i;
    for(i=1;i<=10;i++)
    {
        void * cur_head=heap_alloc(i);
        if(cur_head==NULL)
        {
            printf("Coudn't allocate size %d\n",i);
            continue;
        }
        if(i%2==0)
        {
            bool res=heap_free(cur_head);
            if(!res)
            printf("Coudn't free size %d\n",i);
        }
    }
    printf("Allocated Chunks : \n");
    heap_chunk_dump(&Allocated_Heap_Chunks);
    printf("Freed Chunks : \n");
    heap_chunk_dump(&Freed_Heap_Chunks);
    return 0;
   
}
