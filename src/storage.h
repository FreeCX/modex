#pragma once
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "memory.h"
#include "error.h"

enum data_type {
    DT_NULL = 0,
    DT_STR,
    DT_U32
};

enum error_type {
    ST_OK = 0,
    ST_NULL,
    ST_RANGE,
    ST_MT_UNKNOWN,
    ST_MT_NULL,
    ST_MT_STR,
    ST_MT_U32,
};

typedef struct {
    uint32_t size;
    uint8_t type;
    void * data;
} block_t;

typedef struct {
    size_t data_alloc;
    size_t data_size;
    size_t free_alloc;
    size_t free_size;
    block_t * data;
    size_t * free;
} storage_t;

storage_t * storage_init(size_t size);
uint8_t storage_destroy(storage_t *s);
uint8_t storage_size(storage_t *s, size_t *size);
uint8_t storage_capacity(storage_t *s, size_t *capacity);
uint8_t storage_get_size(storage_t *s, size_t index, size_t *size);
uint8_t storage_push_str(storage_t *s, char *data, size_t *index);
uint8_t storage_push_u32(storage_t *s, uint32_t data, size_t *index);
uint8_t storage_get_u32(storage_t *s, size_t index, uint32_t *output);
uint8_t storage_view_str(storage_t *s, size_t index, char **output);
uint8_t storage_clone_str(storage_t *s, size_t index, char **output);
uint8_t storage_remove(storage_t *s, size_t index);
