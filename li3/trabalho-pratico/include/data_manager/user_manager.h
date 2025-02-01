#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "types/user.h"
#include "types/history.h"
#include "utils/pool.h"
#include "utils/compare.h"
#include "data_manager/music_manager.h"
#include <glib.h>


#define USER_MANAGER_BLOCK_SIZE 60000
#define HISTORY_MANAGER_BLOCK_SIZE 10000
typedef struct UserManager UserManager;

UserManager* __create_user_manager__(void);
void __free_user_manager__(UserManager *manager);
void __add_user__(UserManager* manager,MusicManager* music_manager ,user_t* user);
user_t* __get_user__(UserManager * manager, const char *id);
void __remove_user__(UserManager* manager, const char* id);
GArray* get_user_likes_by_age(UserManager* manager);

int user_manager_has_key(UserManager* manager, const char* key);
GHashTable* get_user_table(UserManager* user_manager);

pool_t* get_user_allocator(UserManager* manager);
pool_t* get_history_allocator(UserManager* manager);

#endif
