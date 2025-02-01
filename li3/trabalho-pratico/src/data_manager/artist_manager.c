#include "data_manager/artist_manager.h"
#include "utils/pool.h"
#include <glib.h>
#include <stdlib.h>
#include "types/artist.h"

struct ArtistManager {
   GHashTable *artist_table;
   pool_t *artist_allocator;
};

ArtistManager *__create_artist_manager__(void) {
    ArtistManager *manager = malloc(sizeof(ArtistManager));
    if (!manager) return NULL;

    manager->artist_table = g_hash_table_new_full(g_str_hash, g_str_equal, free, __free_artist__);
    manager->artist_allocator = pool_init(ARTIST_BLOCK_LENGTH, artist_sizeof());

    return manager;
}

void __free_artist_manager__(ArtistManager *manager) {
    if (!manager) return;

    g_hash_table_destroy(manager->artist_table);
    pool_free(manager->artist_allocator);
    free(manager);
}

void __add_artist__(ArtistManager* manager, artist_t* artist) {
   char* id = get_artist_id(artist);
   g_hash_table_insert(manager->artist_table, id, artist_clone(artist,manager->artist_allocator));
}

artist_t* __get_artist__(ArtistManager* manager, const char *id) {
   return g_hash_table_lookup(manager->artist_table, id);
}

void __remove_artist__(ArtistManager* manager, const char* id) {
   g_hash_table_remove(manager->artist_table, id);
}

GHashTable* __get_artists_hashTable(ArtistManager* manager){
   return manager->artist_table;
}

pool_t* get_artist_allocator(ArtistManager* manager){
   return manager->artist_allocator;
}

void update_artist_discography_duration(ArtistManager* manager,char* key ,duration_t* duration){
   artist_t* artist = __get_artist__(manager, key);
   increment_discography_duration(artist,duration);
}

void update_artist_num_albums(ArtistManager* manager,char* key){
   artist_t* artist = __get_artist__(manager, key);
   increment_artist_num_albums(artist);
}

void update_artist_num_reproducoes(ArtistManager* manager,char* key){
   artist_t* artist = __get_artist__(manager, key);
   increment_artist_num_reproducoes(artist);
}

int artist_manager_has_key(ArtistManager* artist_mng, char* key){
   gboolean contains = g_hash_table_contains(artist_mng->artist_table, key);
   return contains;
}
