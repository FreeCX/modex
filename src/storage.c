#include "storage.h"

// private funcs
size_t storage_realloc(storage_t *s) {
    if (s->free_size > 0) {
        // reuse free blocks
        size_t index = s->free[s->free_size];
        s->free[s->free_size] = 0;
        s->free_size -= 1;
        return index;
    } else {
        if (s->data_size == s->data_alloc) {
            // alloc new memory
            size_t new_size = s->data_alloc * 2;

            block_t * new_block = (block_t *) mem_calloc(new_size, sizeof(block_t));
            memcpy(new_block, s->data, s->data_alloc * sizeof(block_t));
            mem_free(s->data);

            s->data = new_block;
            s->data_alloc = new_size;
        }

        // and return pointer to new block
        size_t index = s->data_size;
        s->data_size += 1;
        return index;
    }
}

uint8_t storage_what_type(uint8_t type) {
    switch (type) {
        case DT_NULL:
            return ST_MT_NULL;
        case DT_STR:
            return ST_MT_STR;
        case DT_U32:
            return ST_MT_U32;
        default:
            return ST_MT_UNKNOWN;
    }
}

// public funcs
__attribute__ ((visibility ("default")))
storage_t * storage_init(size_t size) {
    storage_t * result = (storage_t *) mem_malloc(sizeof(storage_t));

    result->data_size = 0;
    result->data_alloc = size;
    result->data = (block_t *) mem_calloc(size, sizeof(block_t));
    result->free_size = 0;
    result->free_alloc = size;
    result->free = (size_t *) mem_calloc(size, sizeof(size_t));

    return result;
}

__attribute__ ((visibility ("default")))
uint8_t storage_destroy(storage_t *s) {
    if (s == NULL) {
        return ST_NULL;
    }

    for (size_t i = 0; i < s->data_size; i++) {
        if (s->data[i].data != NULL) {
            mem_free(s->data[i].data);
        }
    }
    mem_free(s->data);
    mem_free(s->free);
    mem_free(s);

    return ST_OK;
}

__attribute__ ((visibility ("default")))
uint8_t storage_size(storage_t *s, size_t *size) {
    if (size == NULL) {
        return ST_NULL;
    }

    *size = s->data_size;

    return ST_OK;
}

__attribute__ ((visibility ("default")))
uint8_t storage_capacity(storage_t *s, size_t *capacity) {
    if (capacity == NULL) {
        return ST_NULL;
    }

    *capacity = s->data_alloc;

    return ST_OK;
}

__attribute__ ((visibility ("default")))
uint8_t storage_get_size(storage_t *s, size_t index, size_t *size) {
    if (s == NULL || size == NULL) {
        return ST_NULL;
    }

    if (index < s->data_size) {
        *size = s->data[index].size;
        return ST_OK;
    }

    return ST_RANGE;
}

__attribute__ ((visibility ("default")))
uint8_t storage_push_str(storage_t *s, char *data, size_t *index) {
    if (s == NULL || data == NULL || index == NULL) {
        return ST_NULL;
    }

    size_t str_size = strlen(data) + 1;
    size_t c_index = storage_realloc(s);
    *index = c_index;

    s->data[c_index].data = mem_calloc(str_size, sizeof(char));
    s->data[c_index].type = DT_STR;
    s->data[c_index].size = str_size;
    memcpy(s->data[c_index].data, data, str_size);

    return ST_OK;
}

__attribute__ ((visibility ("default")))
uint8_t storage_push_u32(storage_t *s, uint32_t data, size_t *index) {
    if (s == NULL || index == NULL) {
        return ST_NULL;
    }

    size_t c_index = storage_realloc(s);
    *index = c_index;

    s->data[c_index].data = mem_malloc(sizeof(uint32_t));
    s->data[c_index].type = DT_U32;
    s->data[c_index].size = sizeof(uint32_t);
    *((uint32_t *)(s->data[c_index].data)) = data;

    return ST_OK;
}

__attribute__ ((visibility ("default")))
uint8_t storage_get_u32(storage_t *s, size_t index, uint32_t *output) {
    if (s == NULL || output == NULL) {
        return ST_NULL;
    }

    if (index < s->data_size) {
        if (s->data[index].type == DT_U32) {
            *output = *((uint32_t *)(s->data[index].data));
            return ST_OK;
        }
        return storage_what_type(s->data[index].type);
    }

    return ST_RANGE;
}

__attribute__ ((visibility ("default")))
uint8_t storage_view_str(storage_t *s, size_t index, char **output) {
    if (s == NULL || output == NULL) {
        return ST_NULL;
    }

    if (index < s->data_size) {
        if (s->data[index].type == DT_STR) {
            *output = s->data[index].data;
            return ST_OK;
        }
        return storage_what_type(s->data[index].type);
    }

    return ST_RANGE;
}

__attribute__ ((visibility ("default")))
uint8_t storage_clone_str(storage_t *s, size_t index, char **output) {
    if (s == NULL || output == NULL) {
        return ST_NULL;
    }

    if (index < s->data_size) {
        if (s->data[index].type == DT_STR) {
            memcpy(*output, s->data[index].data, s->data[index].size);
            return ST_OK;
        }
        return storage_what_type(s->data[index].type);
    }

    return ST_RANGE;
}

uint8_t storage_remove(storage_t *s, size_t index) {
    if (s == NULL) {
        return ST_NULL;
    }

    // realloc free buffer
    if (s->free_size == s->free_alloc) {
        size_t new_size = s->free_alloc * 2;

        size_t * new_block = (size_t *) mem_calloc(new_size, sizeof(size_t));
        memcpy(new_block, s->free, s->free_alloc * sizeof(size_t));
        mem_free(s->free);

        s->free = new_block;
        s->free_alloc = new_size;
    }
    // store index as free
    s->free[s->free_size] = index;
    s->free_size += 1;

    // mark as free
    s->data[index].type = DT_NULL;
    s->data[index].size = 0;
    mem_free(s->data[index].data);

    return ST_OK;
}
