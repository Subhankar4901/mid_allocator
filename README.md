# Custom Memory Allocator
This project implements a simple custom memory allocator in C, providing three primary functions for memory management: initializing the allocator, allocating memory on the heap, and freeing allocated memory.
For without Garbage Collection Version : [3d54d4db42eb03cf23c1d0346d81fa2514ed869d](https://github.com/Subhankar4901/mid_allocator/tree/3d54d4db42eb03cf23c1d0346d81fa2514ed869d)
## Functions

### `void init_allocator(void)`
Initializes the memory allocator. Must be called before any allocation or deallocation operations.

### `void* heap_alloc(size_t size)`
Allocates `size` bytes on the heap and returns a pointer to the allocated memory.

### `bool heap_free(void* ptr)`
Frees the memory at the specified location. Returns `true` if successful, `false` otherwise.

## Usage

1. **Initialization**
    ```c
    init_allocator();
    ```

2. **Allocation**
    ```c
    void* ptr = heap_alloc(100); // Allocates 100 bytes
    ```

3. **Deallocation**
    ```c
    bool success = heap_free(ptr);
    ```

## Running Tests

To run the test cases:
```bash
make test
./test
