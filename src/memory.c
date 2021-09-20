#include "memory.h"

const char * error_allocate = "[%s] cannot allocate memory block!\n";

void * mem_malloc(size_t size) {
    void * mem = malloc(size);
    if (mem == NULL) {
        abort_process(error_allocate, "mem_malloc");
        // unreachable
    }
    return mem;
}

void * mem_calloc(size_t count, size_t size) {
    void * mem = calloc(count, size);
    if (mem == NULL) {
        abort_process(error_allocate, "mem_calloc");
        // unreachable
    }
    return mem;
}

void mem_free(void * mem) {
    if (mem == NULL) {
        fprintf(stderr, "[mem_free] cannot deallocate NULL pointer!\n");
        return;
    }
    free(mem);
}
