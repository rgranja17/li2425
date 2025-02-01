#include "data_manager/database_manager.h"

struct Database_manager{
   UserManager* user_manager;
   MusicManager* music_manager;
   ArtistManager* artist_manager;
   AlbumManager* album_manager;
};

int validate_music(music_t *music, Database_manager* db) {
   if (!music || !db) {
      return 1;
   }

   char* album_id = get_music_album_id(music);

   if(!database_manager_has_album(db, album_id)){
      free(album_id);
      return 1;
   }

   free(album_id);

   GArray* artist_ids = get_music_artist_ids(music);
   if (!artist_ids) {
      return 1; 
   }


   for (guint i = 0; i < artist_ids->len; i++) {
      char* id = g_array_index(artist_ids, char*, i);
      if (!database_manager_has_artist(db, id)) {
         for(guint j = 0; j < artist_ids->len; j++){
            free(g_array_index(artist_ids, char*, j));
         }
         g_array_free(artist_ids, TRUE);  
         return 1;
      }
   }

   for (guint i = 0; i < artist_ids->len; i++) {
      free(g_array_index(artist_ids, char*, i));
   }
   g_array_free(artist_ids, TRUE); 

   return 0;
}

int validate_user(user_t* user, Database_manager* db) {
   if (!user) {
      return 1;
   }

   guint len = user_get_array_size(user);
   GArray* tmp_array = get_user_liked_music_ids(user);
   if (tmp_array) { 
      for (guint i = 0; i < len; i++) {
         char *music_id = g_array_index(tmp_array, char*, i);
         if (!database_manager_has_music(db, music_id)) {
            for (guint j = 0; j < tmp_array->len; j++) {
               free(g_array_index(tmp_array, char*, j));
            }
            g_array_free(tmp_array, TRUE); 
            return 0;
         }
      }

      for (guint i = 0; i < tmp_array->len; i++) {
         free(g_array_index(tmp_array, char*, i));
      }
      g_array_free(tmp_array, TRUE);
   }

   return 1;
}

Database_manager *newDatabase(void) {
   Database_manager* new_database = malloc(sizeof(Database_manager));
   new_database->user_manager = __create_user_manager__();
   new_database->music_manager = create_music_manager();
   new_database->artist_manager = __create_artist_manager__();
   new_database->album_manager = __create_album_manager__();
   //new_database->history_manager = __create_history_manager__();
   return new_database;
}


UserManager *getUserManager(Database_manager* db) {
   return db->user_manager;
} 

ArtistManager *getArtistManager(Database_manager* db) {
   return db->artist_manager;
}

MusicManager *getMusicManager(Database_manager* db) {
   return db->music_manager;
}

AlbumManager *getAlbumManager(Database_manager* db) {
   return db->album_manager;
}

/*HistoryManager *getHistoryManager(Database_manager* db) {
   return db->history_manager;
}*/

void database_manager_add_artist(Database_manager* db, artist_t* artist){
   __add_artist__(db->artist_manager,artist);
}

void database_manager_add_music(Database_manager* db, music_t* music){
   add_music(db->music_manager, music);
   GArray* a_ids = get_music_artist_ids(music);
   for(guint i = 0; i < a_ids->len; i++){
      char* id = g_array_index(a_ids, char*, i); 
      //printf("id a:%s\n", id);
      artist_t* artist = __get_artist__(db->artist_manager, id);
      int type = get_artist_type(artist);
      if(type == GROUP){
         GArray* consti = get_artist_id_constituents(artist);
         for(guint i = 0; i < consti->len; i++){
            artist_t* aux = __get_artist__(db->artist_manager,g_array_index(consti, char*, i));
            add_artist_participation(aux,id);
            free(g_array_index(consti, char*, i));
         }
         g_array_free(consti, TRUE);
      }
   }

   for(guint i = 0; i < a_ids->len; i++){
      free(g_array_index(a_ids,char*,i));
   }
   g_array_free(a_ids,TRUE);
}

float get_artist_total_recipe(Database_manager* db, artist_t* artist){
    float group_recipe = 0;
    float individual_recipe = get_artist_recipe(artist) * get_artist_num_reproducoes(artist);

    GArray* group = get_artist_group(artist);

    for(guint i = 0; i < group->len; i++){
      artist_t* aux = __get_artist__(db->artist_manager,g_array_index(group, char*, i));
      GArray* consti = get_artist_id_constituents(aux);
      group_recipe += (get_artist_num_reproducoes(aux) * get_artist_recipe(aux)) / consti->len;
      for(guint i = 0; i < consti->len; i++){
         free(g_array_index(consti,char*,i));
      }
      g_array_free(consti,TRUE);
      free(g_array_index(group, char*, i));
    }
    g_array_free(group,TRUE);

    int type = get_artist_type(artist);

   return type == GROUP ? individual_recipe: individual_recipe + group_recipe;
}

void database_manager_add_user(Database_manager* db, user_t* user){
   __add_user__(db->user_manager,db->music_manager,user);
}

void database_manager_add_album(Database_manager* db, album_t* album){
   __add_album__(db->album_manager,album);
}

/*void database_manager_add_history(Database_manager* db, history_t* history, char* user_id){
   user_t* user = __get_user__(db->user_manager,user_id);
   add_user_history(user,history);
}*/

void database_manager_add_history(Database_manager* db, history_t* history, char* user_id){
   user_t* user = __get_user__(db->user_manager,user_id);
   pool_t* h_allocator = get_history_allocator(db->user_manager);
   history_t* h = history_clone(history,h_allocator);
   add_user_history(user,h);
}

int database_manager_has_artist(Database_manager* db, char* id){
   return artist_manager_has_key(db->artist_manager,id);
}

int database_manager_has_music(Database_manager* db, char* id){
   return music_manager_has_key(db->music_manager, id);
}

int database_manager_has_user(Database_manager* db, char* id){
   return user_manager_has_key(db->user_manager, id);
}

int database_manager_has_album(Database_manager* db, char* id){
   return album_manager_has_key(db->album_manager, id);
}

/*int database_manager_has_history(Database_manager* db, char* id){
   return album_manager_has_key(db->history_manager, id);
}*/

artist_t* database_manager_alloc_new_artist(Database_manager* db){
   return artist_alloc(get_artist_allocator(db->artist_manager));
}

music_t* database_manager_alloc_new_music(Database_manager* db){
   return music_alloc(get_music_allocator(db->music_manager));
}

user_t* database_manager_alloc_new_user(Database_manager* db){
   return user_alloc(get_user_allocator(db->user_manager));
}

album_t* database_manager_alloc_new_album(Database_manager* db){
   return album_alloc(get_album_allocator(db->album_manager));
}

history_t* database_manager_alloc_new_history(Database_manager* db){
   return history_alloc(get_history_allocator(db->user_manager));
}

pool_t* database_get_history_allocator(Database_manager* db){
   return get_history_allocator(db->user_manager);
}

void database_update_artist_duration(Database_manager* db, GArray* ids, duration_t* d){
   for(guint i = 0; i < ids->len; i++){
      char* id = g_array_index(ids,char*,i);
      update_artist_discography_duration(db->artist_manager,id,d);
   }
}

void database_update_artist_num_albums(Database_manager* db, GArray* ids){
   for(guint i = 0; i < ids->len; i++){
      char* id = g_array_index(ids,char*,i);
      update_artist_num_albums(db->artist_manager,id);
   }
}

void database_update_artist_num_reproducoes(Database_manager* db, const char* music_id){
   music_t* music = get_music(db->music_manager, music_id);
   GArray* artist_ids = get_music_artist_ids(music);
   if(!artist_ids) return;
   for(guint i = 0; i < artist_ids->len; i++){
      char* id = g_array_index(artist_ids,char*,i);
      update_artist_num_reproducoes(db->artist_manager,id);
   }

   for (guint i = 0; i < artist_ids->len; i++) {
      free(g_array_index(artist_ids, char*, i));  
   }
   g_array_free(artist_ids, TRUE);
}

void create_album_manager(Database_manager* db, const char* dataset_path){
   char path[256];
   const char* album_path_termination = "/albums.csv";
   snprintf(path, sizeof(path), "%s%s",dataset_path , album_path_termination);

   FILE* file = fopen(path,"r");
   if (!file) {
      perror("Error opening album file\n");
      return;
   }

   GArray *array = g_array_new(FALSE, FALSE, sizeof(char *));
   album_t* album = NULL;
   album = database_manager_alloc_new_album(db);

   int flag = 1, i = 0;
   char* line = NULL;
   while(flag) {
      flag = process_line(file, array, &line);
      if (!flag){
         break;
      }

      if(i == 0){
         i++;
         free(line);
         continue;
      }

      //album = database_manager_alloc_new_album(db);
      create_album_from_line(album, array);
      database_manager_add_album(db,album);
      
      if(get_album_num_artists(album) > 0){
         GArray* artist_ids = get_album_artist_ids(album);
         database_update_artist_num_albums(db,artist_ids);
         
         for (guint i = 0; i < artist_ids->len; i++) {
            free(g_array_index(artist_ids, char*, i));  
         }
         g_array_free(artist_ids, TRUE);
      }

      free(line);
      //__free_album__(album);
   }

   __free_album__(album);

   free_garray_parsed(array);
   fclose(file);
   //printf("Albuns preenchidos");
}

void create_history_manager(Database_manager* db, const char* dataset_path){
   char path[256];
   const char* user_path_termination = "/history.csv";
   snprintf(path, sizeof(path), "%s%s",dataset_path , user_path_termination);

   FILE* file = fopen(path,"r");
   if (!file) {
      perror("Error opening history file\n");
      return;
   }

   FILE* error_file;
   error_file = fopen("resultados/history_errors.csv", "a");

   if (!error_file) {
      perror("Error opening history_errors file\n");
      fclose(file);
      return;
   }

   GArray *array = g_array_new(FALSE, FALSE, sizeof(char *));
   
  

   int flag = 1, i = 0;
   char* line = NULL;
   while(flag) {
      flag = process_line(file, array, &line);

      if (!flag){
         break;
      }

      if(i == 0){
         i++;
         free(line);
         continue;
      }

      history_t* history = NULL;
       history = database_manager_alloc_new_history(db);
      int v = create_history_from_line(history, array);
      
      if(v){
         log_error(error_file,line); 
         free(line);
         continue;
      }


      database_manager_add_history(db,history, g_array_index(array, char*, 1));
      char* music_id = get_history_music_id(history);
      database_update_artist_num_reproducoes(db, music_id);

      free(music_id);
       __free_history__(history);
      free(line);
      
   }
  
   free_garray_parsed(array);
   fclose(file);
   fclose(error_file);
}
void create_artist_manager(Database_manager* db, const char* dataset_path){
   char path[256];
   const char* artist_path_termination = "/artists.csv";
   snprintf(path, sizeof(path), "%s%s",dataset_path , artist_path_termination);

   FILE* file = fopen(path,"r");
   if (!file) {
      perror("Error opening artists file\n");
      return;
   }

   FILE* error_file;
   error_file = fopen("resultados/artists_errors.csv", "a");

   if (!error_file) {
      perror("Error opening artists_errors file\n");
      fclose(file);
      return;
   }

   GArray *array = g_array_new(FALSE, FALSE, sizeof(char *));
   artist_t* artist = NULL;
   artist = database_manager_alloc_new_artist(db);

   int flag = 1, i = 0;
   char* line = NULL;
   while(flag) {
      flag = process_line(file, array, &line);

      if (!flag){
         break;
      }

      if(i == 0){
         i++;
         free(line);
         continue;
      }

      //artist = database_manager_alloc_new_artist(db);
      int v = create_artist_from_line(artist, array);

      if(v){
         log_error(error_file,line); 
         free(line);
         //__free_artist__(artist);
         continue;
      }

      database_manager_add_artist(db,artist);

      free(line);
      //__free_artist__(artist);
   }

   __free_artist__(artist);

   free_garray_parsed(array);
   fclose(file);
   fclose(error_file);
}

void create_music_manager_(Database_manager* db, const char* dataset_path){
   char path[256];
   const char* music_path_termination = "/musics.csv";
   snprintf(path, sizeof(path), "%s%s",dataset_path , music_path_termination);

   FILE* file = fopen(path,"r");
   if (!file) {
      perror("Error opening musics file\n");
      return;
   }

   FILE* error_file;
   error_file = fopen("resultados/musics_errors.csv", "a");

   if (!error_file) {
      perror("Error opening musics_errors file\n");
      fclose(file);
      return;
   }

   GArray *array = g_array_new(FALSE, FALSE, sizeof(char *));
   music_t* music = NULL;
   music = database_manager_alloc_new_music(db);

   int flag = 1, i = 0;
   char* line = NULL;
   while(flag) {
      flag = process_line(file, array, &line);

      if (!flag){
         break;
      }

      if(i == 0){
         i++;
         free(line);
         continue;
      }

      //music = database_manager_alloc_new_music(db);
      int v = create_music_from_line(music, array);

      if(v || validate_music(music,db)){
         log_error(error_file,line); 
         free(line);
         //__free_music__(music);
         continue;
      }

      database_manager_add_music(db,music);
      GArray* aux = get_music_artist_ids(music);
      duration_t* d = get_music_duration_struct(music);
      database_update_artist_duration(db,aux,d);
      for(guint i = 0; i < aux->len; i++){
         free(g_array_index(aux,char*,i));
      }
      g_array_free(aux,TRUE);
      duration_free(d);

      free(line);
      //__free_music__(music);
   }

   __free_music__(music);

   free_garray_parsed(array);
   fclose(file);
   fclose(error_file);
}

void free_database(Database_manager *db) {
   if(db != NULL) {
      free_music_manager(db->music_manager);
      __free_user_manager__(db->user_manager);
      __free_artist_manager__(db->artist_manager);
      __free_album_manager__(db->album_manager);
      //__free_history_manager__(db->history_manager);
      free(db);
   }
}

void create_user_manager_(Database_manager* db, const char* dataset_path){
   char path[256];
   const char* user_path_termination = "/users.csv";
   snprintf(path, sizeof(path), "%s%s",dataset_path , user_path_termination);

   FILE* file = fopen(path,"r");
   if (!file) {
      perror("Error opening users file\n");
      return;
   }

   FILE* error_file;
   error_file = fopen("resultados/users_errors.csv", "a");

   if (!error_file) {
      perror("Error opening users_errors file\n");
      fclose(file);
      return;
   }

   GArray *array = g_array_new(FALSE, FALSE, sizeof(char *));
   user_t* user = NULL;
   user = database_manager_alloc_new_user(db);

   int flag = 1, i = 0;
   char* line = NULL;
   while(flag) {
      flag = process_line(file, array, &line);

      if (!flag){
         break;
      }

      if(i == 0){
         i++;
         free(line);
         continue;
      }

      int v = create_user_from_line(user, array);

      if(v || !validate_user(user,db)){
         log_error(error_file,line); 
         free(line);
         continue;
      }

      database_manager_add_user(db,user);

      free(line);
   }

   __free_user__(user);

   free_garray_parsed(array);
   fclose(file);
   fclose(error_file);
}