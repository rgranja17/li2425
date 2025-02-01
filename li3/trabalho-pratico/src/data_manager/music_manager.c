#include "types/music.h"
#include <glib.h>
#include "data_manager/music_manager.h"

struct MusicManager{
    GHashTable *music_table;
    pool_t* music_allocator;
};

MusicManager* create_music_manager(void) {
    MusicManager *manager = malloc(sizeof(MusicManager));
    manager->music_table = g_hash_table_new_full(g_str_hash, g_str_equal,free,__free_music__);
    manager->music_allocator = pool_init(MUSIC_MANAGER_BLOCK_SIZE, music_sizeof());
    return manager;
}

void free_music_manager(MusicManager *manager) {
    g_hash_table_destroy(manager->music_table);
    pool_free(manager->music_allocator);
    free(manager);
}
    
void add_music(MusicManager *manager, music_t *music) {
    char* id = get_music_id(music);
    g_hash_table_insert(manager->music_table, id, music_clone(music, manager->music_allocator));
}

music_t* get_music(MusicManager *manager, const char *id) {
    return g_hash_table_lookup(manager->music_table, id);
}

void remove_music(MusicManager *manager, const char *id) {
    g_hash_table_remove(manager->music_table, id);
}

GHashTable* get_music_table(MusicManager* manager){
    return manager->music_table;
}

int music_manager_has_key(MusicManager* manager, const char* key){
   gboolean contains = g_hash_table_contains(manager->music_table, key);
   return contains;
}

pool_t* get_music_allocator(MusicManager* manager){
    return manager->music_allocator;
}
