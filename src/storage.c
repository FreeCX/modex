#include "storage.h"

// private funcs
void storage_realloc(storage_t *s) {
    if (s->curr_size == s->alloc_size) {
        size_t new_size = s->alloc_size * 2;

        block_t * new_block = (block_t *) mem_calloc(new_size, sizeof(block_t));
        memcpy(new_block, s->data, s->alloc_size * sizeof(block_t));
        mem_free(s->data);

        s->data = new_block;
        s->alloc_size = new_size;
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
    result->curr_size = 0;
    result->alloc_size = size;
    result->data = (block_t *) mem_calloc(size, sizeof(block_t));
    return result;
}

__attribute__ ((visibility ("default")))
void storage_destroy(storage_t *s) {
    if (s != NULL) {
        for (size_t i = 0; i < s->curr_size; i++) {
            if (s->data[i].data != NULL) {
                mem_free(s->data[i].data);
            }
        }
        mem_free(s->data);
        mem_free(s);
    }
}

__attribute__ ((visibility ("default")))
size_t storage_size(storage_t *s) {
    return s->curr_size;
}

__attribute__ ((visibility ("default")))
size_t storage_capacity(storage_t *s) {
    return s->alloc_size;
}

__attribute__ ((visibility ("default")))
uint8_t storage_get_size(storage_t *s, size_t index, size_t *size) {
    if (index < s->curr_size) {
        *size = s->data[index].size;
        return ST_OK;
    }
    return ST_RANGE;
}

__attribute__ ((visibility ("default")))
size_t storage_push_str(storage_t *s, char *data) {
    storage_realloc(s);

    size_t str_size = strlen(data) + 1;
    size_t last_index = s->curr_size;

    s->data[last_index].data = mem_calloc(str_size, sizeof(char));
    s->data[last_index].type = DT_STR;
    s->data[last_index].size = str_size;
    memcpy(s->data[last_index].data, data, str_size);

    s->curr_size += 1;

    return last_index;
}

__attribute__ ((visibility ("default")))
size_t storage_push_u32(storage_t *s, uint32_t data) {
    storage_realloc(s);

    size_t last_index = s->curr_size;

    s->data[last_index].data = mem_malloc(sizeof(uint32_t));
    s->data[last_index].type = DT_U32;
    s->data[last_index].size = sizeof(uint32_t);
    *((uint32_t *)(s->data[last_index].data)) = data;

    s->curr_size += 1;

    return last_index;
}

__attribute__ ((visibility ("default")))
uint8_t storage_get_u32(storage_t *s, size_t index, uint32_t *output) {
    if (index < s->curr_size) {
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
    if (index < s->curr_size) {
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
    if (index < s->curr_size) {
        if (s->data[index].type == DT_STR) {
            memcpy(*output, s->data[index].data, s->data[index].size);
            return ST_OK;
        }
        return storage_what_type(s->data[index].type);
    }
    return ST_RANGE;
}
