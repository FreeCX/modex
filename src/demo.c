#include "storage.h"

int main() {
    storage_t * s = storage_init(3);

    size_t i1, i2, i3, i4, i5, s1, s2, v3size;
    uint8_t r1, r2, r3, r4, r5;
    uint32_t v2, v4, v5;
    char *v1, *v3;

    // TEST: realloc
    printf("----- REALLOC -----\n");
    storage_push_str(s, "hello", &i1);
    storage_push_u32(s, 42, &i2);
    storage_size(s, &s1);
    storage_capacity(s, &s2);
    printf("size %zu, capacity %zu\n", s1, s2);

    storage_push_str(s, "world", &i3);
    storage_push_u32(s, 37, &i4);
    storage_size(s, &s1);
    storage_capacity(s, &s2);
    printf("size %zu, capacity %zu\n", s1, s2);

    // TEST: get
    r1 = storage_view_str(s, i1, &v1);
    storage_get_size(s, i3, &v3size);
    v3 = mem_malloc(v3size);
    r3 = storage_clone_str(s, i3, &v3);
    r2 = storage_get_u32(s, i2, &v2);
    r4 = storage_get_u32(s, i4, &v4);

    printf("----- GET -----\n");
    printf("status %d, index %zu: '%s'\n", r1, i1, v1);
    printf("status %d, index %zu: %d\n", r2, i2, v2);
    printf("status %d, index %zu: '%s'\n", r3, i3, v3);
    printf("status %d, index %zu: %d\n", r4, i4, v4);

    mem_free(v3);

    // TEST: remove & reuse
    printf("----- REUSE-----\n");
    printf("remove item at index %zu\n", i1);
    printf("remove item at index %zu\n", i4);
    storage_remove(s, i1);
    storage_remove(s, i4);
    storage_push_u32(s, 20, &i1);
    storage_push_str(s, "yes", &i4);
    storage_push_u32(s, 44, &i5);
    r2 = storage_get_u32(s, i1, &v2);
    r4 = storage_view_str(s, i4, &v1);
    r5 = storage_get_u32(s, i5, &v5);
    printf("status %d, index %zu: %d\n", r2, i1, v2);
    printf("status %d, index %zu: '%s'\n", r4, i4, v1);
    printf("status %d, index %zu: %d\n", r5, i5, v5);

    storage_destroy(s);
}
