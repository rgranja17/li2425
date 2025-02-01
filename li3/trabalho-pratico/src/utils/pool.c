#include "utils/pool.h"
/*
*   pool struct:
*       -> blocks:: array of pointers to each block memory (array of linked lists)
*       -> block_size:: defines the number of items that each memory block supports
*       -> item_size:: defines the size of each item present in the block
*       -> block_number:: stores the number of blocks currently allocated
*   Considerations: 
*       -> Each block has a capacity
*       -> Blocks are defined as linked lists
*       -> The blocks array grows over time 
*/
struct pool{
    GPtrArray*   blocks;
    size_t       item_size;
    int          block_length;
    int          last_block_items;
};

int pool_alloc_new_block(pool_t* pool){
    //uint8_t* new_block = malloc(pool->block_length * pool->item_size);
    uint8_t* new_block = calloc(pool->block_length, pool->item_size);
    if(!new_block){
        return 1;
    }

    g_ptr_array_add(pool->blocks, new_block);
    pool->last_block_items = 0;

    return 0;
}

void* pool_alloc_item(pool_t* pool){
    if(pool->last_block_items == pool->block_length){
        if(pool_alloc_new_block(pool)){
            printf("Falha na alocação de novo bloco\n");
            return NULL; 
        }
    }

    uint8_t* current_block = (uint8_t*) g_ptr_array_index(pool->blocks, pool->blocks->len - 1);

    size_t offset = pool->item_size * pool->last_block_items;
    void* item = current_block + offset;

    pool->last_block_items++;
    return item;
}


void* pool_insert_item(pool_t* pool,const void* item){
    void* addr = pool_alloc_item(pool);
    if(!addr){
        return NULL;
    }
    memcpy(addr, item, pool->item_size);
    return addr;
}

pool_t* pool_init(int block_length, size_t item_size){
    pool_t* new_pool = malloc(sizeof(pool_t));
    if(!new_pool){
        return NULL;
    }

    new_pool->blocks = g_ptr_array_new_with_free_func(free);
    new_pool->item_size = item_size;
    new_pool->block_length = block_length;
    new_pool->last_block_items = 0;

    if(pool_alloc_new_block(new_pool)){
        free(new_pool);
        return NULL;
    }

    return new_pool;
} 

int pool_free(pool_t *pool) {
    g_ptr_array_unref(pool->blocks);
    free(pool);
    return 0;
}