#include "data_manager/album_manager.h"


struct AlbumManager {
   GHashTable *album_table;
   pool_t* album_allocator;
};

AlbumManager* __create_album_manager__(void) {
   AlbumManager * manager = malloc(sizeof(AlbumManager));
   manager->album_table = g_hash_table_new_full(g_str_hash,g_str_equal, free, __free_album__);
   manager->album_allocator = pool_init(ALBUM_MANAGER_BLOCK_SIZE,album_sizeof());
   return manager;
}

void __free_album_manager__(AlbumManager *manager) {
   g_hash_table_destroy(manager->album_table);
   pool_free(manager->album_allocator);
   free(manager);
}

void __add_album__(AlbumManager* album_manager, album_t* album) {
   char* id = get_album_id(album);
   g_hash_table_insert(album_manager->album_table, id, album_clone(album, album_manager->album_allocator));
}

album_t* __get_album__(AlbumManager* manager, const char *id) {
   return g_hash_table_lookup(manager->album_table,id);
}

void __remove_album__(AlbumManager* manager, const char* id) {
   g_hash_table_remove(manager->album_table,id);
}

int album_manager_has_key(AlbumManager* manager, const char* key){
   return g_hash_table_contains(manager->album_table, key);
}

GHashTable* get_album_table(AlbumManager* album_manager) {
   return album_manager->album_table;
}

pool_t* get_album_allocator(AlbumManager* manager){
   return manager->album_allocator;
}