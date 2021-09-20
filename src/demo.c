#include "storage.h"

int main() {
    storage_t * s = storage_init(3);

    size_t i1 = storage_push_str(s, "hello");
    size_t i2 = storage_push_u32(s, 42);
    printf("size %zu, capacity %zu\n", storage_size(s), storage_capacity(s));

    size_t i3 = storage_push_str(s, "world");
    size_t i4 = storage_push_u32(s, 37);
    printf("size %zu, capacity %zu\n", storage_size(s), storage_capacity(s));

    char *v1 = NULL;
    uint8_t r1 = storage_view_str(s, i1, &v1);

    size_t v3size = 0;
    storage_get_size(s, i3, &v3size);
    char *v3 = mem_malloc(v3size);
    uint8_t r3 = storage_clone_str(s, i3, &v3);

    uint32_t v2 = 0, v4 = 0;
    uint8_t r2 = storage_get_u32(s, i2, &v2);
    uint8_t r4 = storage_get_u32(s, i4, &v4);

    printf("status %d, index %zu: '%s'\n", r1, i1, v1);
    printf("status %d, index %zu: %d\n", r2, i2, v2);
    printf("status %d, index %zu: '%s'\n", r3, i3, v3);
    printf("status %d, index %zu: %d\n", r4, i4, v4);

    mem_free(v3);

    storage_destroy(s);
}
