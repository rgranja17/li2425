
#ifndef POOL_H
#define POOL_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include <glib.h>

typedef struct pool pool_t;
pool_t* pool_init(int block_length, size_t item_size);
void* pool_insert_item(pool_t* pool,const void* item);
void* pool_alloc_item(pool_t* pool);
int pool_free(pool_t* pool);


#endif
