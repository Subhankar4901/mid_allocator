# mid_alloc  
![GitHub Release](https://img.shields.io/github/v/release/Subhankar4901/mid_allocator?link=https%3A%2F%2Fgithub.com%2FSubhankar4901%2Fmid_allocator%2Freleases%2Ftag%2Fv1.0.0)

This project implements a simple custom memory allocator and a garbage collector in C, providing three primary functions for memory management: allocating memory on the heap, freeing allocated memory and garbage collection.
<br>

## User APIs

### `void init_allocator(size_t memcap_in_bytes)`
Initializes the memory allocator with `memcap_in_bytes` amount of pre-allocated memory. If input is zero it will allocate a pre-defined amount of memory.Must be called before any allocation or deallocation operations. It only need to call once.

### `void* mid_alloc(size_t size_in_bytes)`
Allocates `size_in_bytes` amount of bytes on the heap and returns a pointer to the allocated memory. All allocations happens from the amount of bytes pre-allocated by `void init_allocator(size_t)`.

### `bool mid_free(void* ptr)`
Frees the memory at the specified location. Returns `true` if successful, `false` otherwise.

### `void mid_collect()`
Garbage collector. When called, it will free all the unreachable memory which are allocated before calling it.

## Usage

1. **Initialization**

    Pre-allocating pre-defined amount of bytes of memory
    ```c
    #include "allocator.h"
    int main(){
        init_allocator(0);
    }
    ```
    Pre-allocating 4096 bytes of memory.
    ```c
    #include "allocator.h"
    int main(){
        init_allocator(4096);
    }
    ```

2. **Allocation**
    ```c
    #include "allocator.h"
    typedef struct Node{
        int val;
        struct Node * next;
    }Node_t;
    int main(){
        init_allocator(0);
        Node_t*head =(Node_t*) mid_alloc(sizeof(Node_t));
        if(head!=NULL)
        printf("Successfully created linked list head.");
        else
        printf("Failed to create linked list head.");
    }
    ```

3. **Deallocation**
    ```c
    #include "allocator.h"
    int main(){
        init_allocator(0);
        char * str=(char *)mid_alloc(10 * sizeof(char));
        bool result = mid_free(ptr);
        if(result)
            printf("String de-allocated successfully.");
        else
            printf("Failed to de-allocate the string.")
    }
    ```
4. **Garbage Collection**
    ```c
    #include "allocator.h"
    int main(){
        init_allocator(0);
        int * arr=(int*)mid_alloc(5*sizeof(int));
        char * str=(char *)mid_alloc(10 * sizeof(char));
        arr=NULL
        mid_collect();
        // All the memory allocated by 'arr' has been deallocated but
        // not the string 'str' as it still reachable.
    }
    ```

## Running Tests

To run the test cases, add your test case in the folder tests in similar way I have written (`array_test.c`, `tree_test.c` etc.) then import your testing function to `test.c` via `test.h` then run following : 

```bash
make test
./test
```
## Usage
Compile the project into a shared library using :
```bash
make lib
```
It will create `libmid_alloc.so`. then dynamically link the shared library and run your program.
```c
// main.c
#include "allocator.h"
typedef struct Node{
    int val;
    struct Node * next;
}Node_t;
int main(){
    init_allocator(0);
    Node_t*head =(Node_t*) mid_alloc(sizeof(Node_t));
    if(head!=NULL)
    printf("Successfully created linked list head.");
    else
    printf("Failed to create linked list head.");
}
```
```bash
gcc main.c -L <Path to library> -lmid_alloc -o main
LD_LIBRARY_PATH=. ./main
```
Or you can install the library using :
```bash
sudo make install
```
It will copy the created shared library to `/usr/local/lib` and run `ldconfig` then run :
```bash
gcc main.c -lmid_alloc -o main
./main
```