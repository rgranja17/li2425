#include "data_manager/user_manager.h"


struct UserManager {
   GHashTable *user_table;
   GArray* likes_by_age;
   pool_t* user_allocator;
   pool_t* history_allocator;
};

UserManager* __create_user_manager__(void) {
   UserManager * manager = malloc(sizeof(UserManager));
   manager->user_table = g_hash_table_new_full(g_str_hash,g_str_equal, free, __free_user__);
   manager->user_allocator = pool_init(USER_MANAGER_BLOCK_SIZE,user_sizeof());
   manager->history_allocator = pool_init(HISTORY_MANAGER_BLOCK_SIZE,history_sizeof());
   manager->likes_by_age = g_array_new(FALSE, FALSE, sizeof(GArray*));
   return manager;
}

void __free_user_manager__(UserManager *manager) {
    g_hash_table_destroy(manager->user_table);
    pool_free(manager->user_allocator);
    pool_free(manager->history_allocator);
    
    for (guint i = 0; i < manager->likes_by_age->len; i++) {
        GArray* music_genres = g_array_index(manager->likes_by_age, GArray*, i);
        
        for (guint j = 0; j < music_genres->len; j++) {
            music_genre_t* aux = g_array_index(music_genres, music_genre_t*, j);
            music_genre_free(aux);
        }
        
        g_array_free(music_genres, TRUE);
    }
    
    g_array_free(manager->likes_by_age, TRUE);
    free(manager);
}

void __add_user__(UserManager* manager,MusicManager* music_manager ,user_t* user) {
   char* name = get_user_name(user);
   g_hash_table_insert(manager->user_table,name,user_clone(user,manager->user_allocator,manager->history_allocator));
   date_t* birth_date = get_user_birth_date_struct(user);
   int age = calculate_age(birth_date);
   free(birth_date);

   if ((int)manager->likes_by_age->len <= age) {
      guint previous_size = manager->likes_by_age->len;
      g_array_set_size(manager->likes_by_age, age + 1);
      for (guint i = previous_size; (int)i <= age; i++) {
         GArray* aux = music_genre_alloc_array();
         g_array_index(manager->likes_by_age, GArray*, i) = aux;
      }
   }


   GArray* user_likes = get_user_liked_music_ids(user);

   for(guint i = 0; i < user_likes->len; i++){
      char* music_id = g_array_index(user_likes, char*, i);
      music_t* music = get_music(music_manager, music_id);
      int genre = get_music_genre(music);
      GArray* genre_by_age = g_array_index(manager->likes_by_age, GArray*, age);
      music_genre_t* genre_struct = g_array_index(genre_by_age, music_genre_t*, genre);
      increase_music_likes(genre_struct);
   }

   for(guint i = 0; i < user_likes->len; i++){
      free(g_array_index(user_likes, char*, i));
   }
   g_array_free(user_likes, TRUE);

}

GArray* get_user_likes_by_age(UserManager* manager){
   return manager->likes_by_age;
}

user_t* __get_user__(UserManager * manager, const char *id) {
   return g_hash_table_lookup(manager->user_table,id);
}

void __remove_user__(UserManager* manager, const char* id) {
   g_hash_table_remove(manager->user_table,id);
}

int user_manager_has_key(UserManager* manager, const char* key){
   return g_hash_table_contains(manager->user_table, key);
}


GHashTable* get_user_table(UserManager* user_manager) {
    return user_manager->user_table; 
}

pool_t* get_user_allocator(UserManager* manager){
   return manager->user_allocator;
}

pool_t* get_history_allocator(UserManager* manager) {
    return manager->history_allocator;
}
