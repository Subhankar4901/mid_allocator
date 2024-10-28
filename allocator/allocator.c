#include<stdio.h>
#include<unistd.h>
#include<sys/mman.h>
#include "../utils/utils.h"
#define DEFAULT_MEM_CAP_IN_WORDS 131072 // 1MB = 131072 * 8 * 1024 * 1024 Bytes
#define DEFAULT_MEM_CHUNK_CAP 2048
#define MAP_ANONYMOUS 0x20
typedef struct{
    void * start;
    size_t size;
    size_t memory_left;
    size_t max_chunk_list_size;
    HeapChunk_List *allocated_list;
    HeapChunk_List *free_list;
    bool reachable[DEFAULT_MEM_CHUNK_CAP];
    const void ** main_func_stack_base_ptr;
}MemoryManager;
static MemoryManager Memman={0};

/**
 * @brief Initializes the allocator.
 * 
 * It takes a non-negetive integer and pre-allocates that amount of bytes on the heap.
 * Your program can then can ask for memory using `mid_alloc()`.
 * If provided `0` as input it will allocate a pre-defined amount
 * of memory `(1MB)`. Always call it from main function.
 * 
 * @param memcap_in_bytes a non negetive integer if provided `0` then pre-defined amount of
 * bytes(1MB) will be allocated.
 * 
 * @return void
*/
void init_allocator(size_t memcap_in_bytes)
{
    Memman.main_func_stack_base_ptr=(const void**)__builtin_frame_address(1);
    size_t mem_cap_in_words=DEFAULT_MEM_CAP_IN_WORDS;

    if(memcap_in_bytes!=0)
    {
        int page_size=getpagesize();
        memcap_in_bytes=((memcap_in_bytes+page_size-1)/page_size)*page_size;
        mem_cap_in_words=memcap_in_bytes/sizeof(void*);
    }
    Memman.size=(mem_cap_in_words*sizeof(void*));
    Memman.memory_left=Memman.size;
    Memman.max_chunk_list_size=DEFAULT_MEM_CHUNK_CAP;
    Memman.allocated_list=(HeapChunk_List*)mmap(NULL,sizeof(HeapChunk_List),PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANONYMOUS,-1,0);
    Memman.free_list=(HeapChunk_List*)mmap(NULL,sizeof(HeapChunk_List),PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANONYMOUS,-1,0);
    HeapChunk * allocated_heap_chunks_arr=(HeapChunk*)mmap(NULL,DEFAULT_MEM_CHUNK_CAP*sizeof(HeapChunk),PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANONYMOUS,-1,0);
    HeapChunk * freed_heap_chunks_arr=(HeapChunk*)mmap(NULL,DEFAULT_MEM_CHUNK_CAP*sizeof(HeapChunk),PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANONYMOUS,-1,0);
    Memman.allocated_list->Heap_Chunks=allocated_heap_chunks_arr;
    Memman.free_list->Heap_Chunks=freed_heap_chunks_arr;
    Memman.allocated_list->len=0;
    Memman.start=mmap(NULL,Memman.size,PROT_READ | PROT_WRITE,MAP_PRIVATE | MAP_ANONYMOUS,-1,0);
    Memman.free_list->Heap_Chunks[0].start=Memman.start;
    Memman.free_list->Heap_Chunks[0].size=Memman.size;
    Memman.free_list->len=1;
    LOG(INFO,"Allocator has been initialized.");
}

/**
 * @brief Allocates memory from pre-allocated bytes by `init_allocator()`.
 * 
 * It takes a non-negetive integer indicating the amount of bytes to be allocated,
 * If failed returns `NULL`. Also if input is `0` it returns `NULL`.
 * 
 * @param size_in_bytes A non-negetive, non-zero integer.
 * 
 * @return `void *` if it succeeds to allocate the input amount of memory.
 * 
 * @returns `NULL` if it fails.
*/
void * mid_alloc(size_t size_in_bytes)
{
    if(size_in_bytes<=0)
    return NULL;
    size_t size_in_words=((size_in_bytes+sizeof(uintptr_t)-1)/sizeof(uintptr_t));
    size_in_bytes=size_in_words*sizeof(uintptr_t);
    if(Memman.memory_left<size_in_bytes)
    {
        LOG(ERROR,"No more memory left in allocator. Either free the memory by calling heap_free() or heap_collect()(expensive).");
        return NULL;
    }
    merge_freed_chunks(Memman.free_list);
    for(size_t i=0;i<Memman.free_list->len;i++)
    {
        HeapChunk cur_chunk=Memman.free_list->Heap_Chunks[i];
        if(cur_chunk.size>=size_in_bytes)
        {
            HeapChunk remove_res= heap_chunk_remove(Memman.free_list,cur_chunk.start,i);
            if(remove_res.start==NULL && remove_res.size==0)
            {
                LOG_SYS(ERROR,"Failed to remove a free chunk from available chunks.");
                LOG(ERROR,ALLOC_ERR_MSG);
                return NULL;
            }
            HeapChunk required_chunk={
                .start=cur_chunk.start,
                .size=size_in_bytes
            };
            if(cur_chunk.size>size_in_bytes)
            {
            HeapChunk tail_chunk={
                .start=cur_chunk.start+size_in_bytes,
                .size=cur_chunk.size-size_in_bytes
            };
            void * _tail_insertion_res_= heap_chunk_insert(Memman.free_list,&tail_chunk,Memman.max_chunk_list_size);
            if(_tail_insertion_res_==NULL)
            {
                LOG_SYS(ERROR,"Failed to insert the tailing chunk (after removing the required chunk) back to Free chunk list.");
                LOG(ERROR,ALLOC_ERR_MSG);
                return NULL;
            }
            }
            if(Memman.allocated_list->len==Memman.max_chunk_list_size)
            {

                LOG_SYS(ERROR,"Allocated heap chunk lists capacity had been hit.");
                LOG(ERROR,ALLOC_ERR_MSG);
                return NULL;
            }
            HeapChunk * res_chunk=heap_chunk_insert(Memman.allocated_list,&required_chunk,Memman.max_chunk_list_size);
            if (res_chunk)
            {
            Memman.memory_left-=size_in_bytes;
            return res_chunk->start;
            }
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
/**
 * @brief Frees allready allocated memory.
 * 
 * @param ptr A pointer of the memory to be freed.
 * 
 * @returns `true` if succeeds to free the memory.
 * @returns `false` if fails.
*/
bool mid_free(void * ptr)
{

    HeapChunk remove_res=heap_chunk_remove(Memman.allocated_list,ptr,-1);
    if(remove_res.start==NULL)
    {
    LOG_SYS(ERROR,"Failed to remove the chunk from allocated chunk list.");
    LOG(ERROR,DEALLOC_ERR_MSG);
    return false;
    }
    if(Memman.free_list->len==Memman.max_chunk_list_size)
    {
        LOG_SYS(ERROR,"Free heap chunk lists capacity had been hit.");
        LOG(ERROR,DEALLOC_ERR_MSG);
        return false;
    }
    void * insert_res=heap_chunk_insert(Memman.free_list,&remove_res,Memman.max_chunk_list_size);
    if(insert_res==NULL)
    {
    LOG_SYS(ERROR,"Failed to insert the freed chunk into freed chunk list.");
    LOG(ERROR,DEALLOC_ERR_MSG);
    return false;
    }
    Memman.memory_left+=remove_res.size;
    return true;
}
static void mark_reachable(const void** start,const void **end){
    for(;start<end;start++)
    {
        const void*ptr=(const void*)*start;
        for(size_t i=0;i<Memman.allocated_list->len;i++)
        {
            HeapChunk chunk=Memman.allocated_list->Heap_Chunks[i];
            if(chunk.start<=ptr && ptr<chunk.start+chunk.size)
            {
                if(!Memman.reachable[i])
                {
                    Memman.reachable[i]=true;
                    mark_reachable((const void**)chunk.start,chunk.start+chunk.size);
                }
            }
        }
    }
}
/**
 * @brief Garbage collector.
 * 
 * Frees all unreachable memory allocations till the call to function. Always call it from main function, otherwise
 * it may have undefined behabiour.
 * 
*/
void mid_collect(){
    const void**start=(const void**)__builtin_frame_address(0);
    for(int i=0;i<Memman.max_chunk_list_size;i++)
        Memman.reachable[i]=false;
    void * to_be_freed_chunks[Memman.max_chunk_list_size];
    size_t no_of_to_be_freed_chunks=0;
    mark_reachable(start,Memman.main_func_stack_base_ptr);
    for(size_t i=0;i<Memman.allocated_list->len;i++)
    {
        if(!Memman.reachable[i])
        {
            to_be_freed_chunks[no_of_to_be_freed_chunks]=(void *)Memman.allocated_list->Heap_Chunks[i].start;
            no_of_to_be_freed_chunks++;
        }
    }
    for(size_t i=0;i<no_of_to_be_freed_chunks;i++)
        mid_free(to_be_freed_chunks[i]);
}
/**
 * @brief Prints all the memory allocations.
 * Helps in debugging. 
*/
void allocated_mem_dump()
{
    printf("Allocated Chunks(%zu) :\n",Memman.allocated_list->len);
     for(size_t i=0;i<Memman.allocated_list->len;i++)
    {
        printf("Chunk of Size : %zu is at location : %p\n",Memman.allocated_list->Heap_Chunks[i].size,
        Memman.allocated_list->Heap_Chunks[i].start);
    }
}
/**
 * @brief Prints all the free memory.
 * Helps in debugging.
 */
void free_mem_dump()
{
    printf("Freed Chunks(%zu) :\n",Memman.free_list->len);
     for(size_t i=0;i<Memman.free_list->len;i++)
    {
        printf("Chunk of Size : %zu is at location : %p\n",Memman.free_list->Heap_Chunks[i].size,Memman.free_list->Heap_Chunks[i].start);
    }
    
}