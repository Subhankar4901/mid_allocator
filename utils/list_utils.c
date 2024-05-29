#include<stdio.h>
#include "utils.h"
int heap_chunk_query(HeapChunk_List*list,void*ptr)
{
    int lo=0;
    int hi=list->len;
    while(lo<=hi)
    {
        int mid=lo+(hi-lo)/2;
        HeapChunk cur_chunk=list->Heap_Chunks[mid];

        if(cur_chunk.start==ptr)
        return mid;
        else if(cur_chunk.start<ptr)
        lo=mid+1;
        else
        hi=mid-1;
    }
    return -1;
}
int insert_pos(HeapChunk_List*list,HeapChunk*ptr)
{
    int lo=0;
    int hi=list->len;
    while(lo<hi)
    {
        int mid=(hi+lo)/2;
        HeapChunk cur_chunk=list->Heap_Chunks[mid];
        if(cur_chunk.start<=ptr->start)
        {
        hi=mid-1;
        }
        else
        lo=mid+1;
    }
    return lo;
}
void * heap_chunk_insert(HeapChunk_List*list,HeapChunk*ptr)
{
    if(list->len==HEAP_CHUNK_CAP)
      return NULL;
    int idx=insert_pos(list,ptr->start);
    for(size_t i=list->len;i>idx;i--)
    {
        list->Heap_Chunks[i]=list->Heap_Chunks[i-1];
    }
    list->Heap_Chunks[idx]=*ptr;
    list->len+=1;
    return ptr;

}
HeapChunk heap_chunk_remove(HeapChunk_List*list,void*ptr)
{
    HeapChunk res={0};
    if(list->len==0)
    {
    printf("Allocated chunks list is empty.\n");
    return res;
    }
    int idx=heap_chunk_query(list,ptr);
    if(idx==-1)
    {
    printf("Chunk is not allocated.\n");
    printf("Couldn't find %p location\n",ptr);
    return res;
    }
    res=list->Heap_Chunks[idx];
    for(size_t i=idx+1;i<list->len;i++)
    {
        list->Heap_Chunks[i-1]=list->Heap_Chunks[i];
    }
    
    list->Heap_Chunks[list->len-1].size=0;
    list->Heap_Chunks[list->len-1].start=NULL;
    list->len-=1;
    return res;

}