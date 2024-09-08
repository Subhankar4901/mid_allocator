#ifndef UTILS_H
#define UTILS_H
#include<stdbool.h>
#define HEAP_SIZE 640000 //640KB;
#define HEAP_CHUNK_CAP 4000 
#define ALLOC_ERR_MSG "Failed to allocate memory."
#define DEALLOC_ERR_MSG "Failed to de-allocate memory."
#define CHECK_SYS_LOG_MSG "[INFO] Check System logs at /var/log/syslog or /var/log/messages (CentOS,RHEL) for detailed error.\n"
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
typedef enum{
    INFO,
    WARNING,
    ERROR,
    UNKNOWN
}LogLevel;
void heap_chunk_dump(HeapChunk_List*list);
int heap_chunk_query(HeapChunk_List* list,void*ptr);
void* heap_chunk_insert(HeapChunk_List* list,HeapChunk*ptr);
HeapChunk heap_chunk_remove(HeapChunk_List* list,void*ptr,int index);
void merge_freed_chunks(HeapChunk_List * list);
char * LogLevelToString(LogLevel level);
void Log_Msg(LogLevel level,const char * msg,const char * file,int line);
void Log_Sys(LogLevel level,const char * msg,const char * file,int line);
#define LOG(level,msg) Log_Msg(level,msg,__FILE__,__LINE__)
#define LOG_SYS(level,msg) Log_Sys(level,msg,__FILE__,__LINE__);
#endif