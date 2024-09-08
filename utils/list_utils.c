#include<stdio.h>
#include "utils.h"
void heap_chunk_dump(HeapChunk_List * list)
{
    printf("Chunks(%zu) :\n",list->len);
     for(size_t i=0;i<list->len;i++)
    {
        printf("Chunk of Size : %zu is at location : %p\n",list->Heap_Chunks[i].size,list->Heap_Chunks[i].start);
    }   
}
int heap_chunk_query(HeapChunk_List*list,void*ptr)
{
    int lo=0;
    int hi=list->len;
    while(lo<=hi)
    {
        int mid=(hi+lo)/2;
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
void * heap_chunk_insert(HeapChunk_List*list,HeapChunk*ptr)
{
    if(list->len==HEAP_CHUNK_CAP)
      return NULL;
    list->Heap_Chunks[list->len]=*ptr;
    int cur_idx=(list->len);
    while(cur_idx>0 && ptr->start<(list->Heap_Chunks[cur_idx-1].start))
    {
        HeapChunk temp=list->Heap_Chunks[cur_idx];
        list->Heap_Chunks[cur_idx]=list->Heap_Chunks[cur_idx-1];
        list->Heap_Chunks[cur_idx-1]=temp;
        cur_idx--;
    }
    list->len++;
    return ptr;

}
HeapChunk heap_chunk_remove(HeapChunk_List*list,void*ptr,int index)
{
    HeapChunk res={0};
    if(list->len==0)
    {
    LOG_SYS(ERROR,"Given list is empty.Couldn't remove chunk.");
    return res;
    }
    int idx;
    if(index!=-1)
        idx=index;
    else
        idx=heap_chunk_query(list,ptr);
    if(idx==-1)
    {
    char buffer[70];
    snprintf(buffer,sizeof(buffer),"Couldn't find %p location in chunk list.",ptr);
    LOG_SYS(ERROR,(const char *)buffer);
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
bool merge_consecutive(HeapChunk_List*list,int i,int j)
{
    if(i>=list->len || j>=list->len)
    return false;
    if(list->Heap_Chunks[j].start==list->Heap_Chunks[i].start+list->Heap_Chunks[i].size)
    {
        list->Heap_Chunks[i].size+=list->Heap_Chunks[j].size;
        HeapChunk remove_res= heap_chunk_remove(list,list->Heap_Chunks[j].start,j);
        if(remove_res.start==NULL)
        return false;
        return true;
    }
    return false;
}
void merge_freed_chunks(HeapChunk_List*list)
{
    for(size_t i =0;i<list->len;i++)
    {
        while(merge_consecutive(list,i,i+1))
        continue;
    }
}