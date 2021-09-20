#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "error.h"

void * mem_malloc(size_t size);
void * mem_calloc(size_t count, size_t size);
void mem_free(void * mem);
