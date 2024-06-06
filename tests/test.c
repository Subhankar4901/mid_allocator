#include<stdio.h>
#include "../allocator/allocator.h"
int main()
{
    init_allocator();
    for(int i=1;i<=10;i++)
    {
        void * alloc_res=heap_alloc(i);
        if(alloc_res==NULL)
        {
            printf("Failed to allocate %d bytes",i);
            return 1;
        }
        if(i%2==0)
        {
            bool free_res=heap_free(alloc_res);
            if(!free_res)
            {
                printf("Failed to allocate %p location",alloc_res);
                return 1;
            }
        }
    }
    printf("Allocated Chunks : \n");
    heap_chunk_dump(&Allocated_Heap_Chunks);
    printf("Freed Chunks : \n");
    heap_chunk_dump(&Freed_Heap_Chunks);
    return 0;
}