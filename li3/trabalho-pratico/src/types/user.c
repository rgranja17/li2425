#include "types/user.h"

struct user_t {
    char *username;
    char *email;
    char *first_name;
    char *last_name;
    date_t* birth_date;
    char *country;
    stype_t subscription_type : 1;
    GArray *liked_music_ids;

    GArray* user_history;
};

size_t user_sizeof(void) {return sizeof(user_t);}

user_t* user_alloc(pool_t* user_allocator){
    user_t* new_user = pool_alloc_item(user_allocator);
    if(!new_user)
        return NULL;
    new_user->liked_music_ids = g_array_new(FALSE,FALSE,sizeof(char*));
    new_user->user_history = g_array_new(FALSE,FALSE,historyPTR_sizeof());
    new_user->birth_date = malloc(date_sizeof());
    return new_user;
}

user_t* user_clone(user_t* user, pool_t* user_allocator, pool_t* history_allocator) {
    if (!user || !user_allocator) return NULL;

    user_t* new_user = user_alloc(user_allocator);
    if (!new_user) return NULL;

    new_user->username = get_user_name(user);
    new_user->email = get_user_email(user);
    new_user->first_name = get_user_first_name(user);
    new_user->last_name = get_user_last_name(user);
    new_user->country = get_user_country(user);
    new_user->subscription_type = get_user_subscription_type(user);
    
    if (!new_user->liked_music_ids) {
        free(new_user->username);
        free(new_user->email);
        free(new_user->first_name);
        free(new_user->last_name);
        free(new_user->country);
        free(new_user);
        return NULL;
    }

    date_set_day(new_user->birth_date, (int)date_get_day(user->birth_date));
    date_set_month(new_user->birth_date, (int)date_get_month(user->birth_date));
    date_set_year(new_user->birth_date, (int)date_get_year(user->birth_date));

    if (user->liked_music_ids != NULL) {
        for (guint i = 0; i < user->liked_music_ids->len; i++) {
            char *id = g_array_index(user->liked_music_ids, char*, i);
            char *id_copy = strdup(id);
            if (id_copy) {
                g_array_append_val(new_user->liked_music_ids, id_copy);
            }
        }
    }

    if (user->user_history != NULL) {
        for (guint i = 0; i < user->user_history->len; i++) {
            history_t *history = g_array_index(user->user_history, history_t*, i);
            history_t *history_copy = history_clone(history, history_allocator);
            if (history_copy) {
                g_array_append_val(new_user->user_history, history_copy);
            }
        }
    } 

    return new_user;
}

int is_valid_domain(const char *domain) {
    const char* dot_pos = strchr(domain, '.');
    if (!dot_pos || dot_pos == domain) return 0;

    for (const char *p = domain; p < dot_pos; p++) {
        if (!islower(*p)) return 0;
    }

    const char* rstring = dot_pos + 1;
    int rstring_len = strlen(rstring);
    if (rstring_len < 2 || rstring_len > 3) return 0;

    for (const char *p = rstring; *p; p++) {
        if (!islower(*p)) return 0;
    }

    return 1;
}

int is_valid_username(const char *username) {
    for (const char *p = username; *p; p++) {
        if (!isalnum(*p)) return 0;
    }
    return 1;
}

int set_user_name(user_t *user, char *name) {
    if(!user || !name) return 1;
    if(strlen(name) == 0) return 1;

    if(user->username){
        free(user->username);
    }
    user->username = strdup(name);
    return 0;
}

int set_user_email(user_t *user, char *email) {
    if (!email) return 1;
    if (strlen(email) == 0) return 1;
    
    const char* at_pos = strchr(email, '@');
    if (!at_pos || at_pos == email) return 1;

    char username[at_pos - email + 1];
    strncpy(username, email, at_pos - email);
    username[at_pos - email] = '\0';
    if (!is_valid_username(username)) return 1;

    const char* domain = at_pos + 1;
    if (!is_valid_domain(domain)) return 1;

    if(user->email){
        free(user->email);
    }
    user->email = strdup(email);

    return 0; 
}

int set_user_first_name(user_t *user, char *first_name) {
    if(!user || !first_name) return 1;
    if(strlen(first_name) == 0) return 1;

    if(user->first_name){
        free(user->first_name);
    }
    user->first_name = strdup(first_name);
    return 0;
}

int set_user_last_name(user_t *user, char *last_name) {
    if(!user || !last_name) return 1;
    if(strlen(last_name) == 0) return 1;

    if(user->last_name){
        free(user->last_name);
    }
    user->last_name = strdup(last_name);
    return 0;
}

int set_user_birth_date_from_string(user_t *user,char *birth_date) {
    if(!user || !birth_date) return 1;
    if(strlen(birth_date) == 0) return 1;

    if(date_parse_from_str(user->birth_date,birth_date)) return 1;
    return 0;
}

int set_user_birth_date(user_t *user,date_t *birth_date) {
    if(!user || !birth_date) return 1;

    if (date_set_day(user->birth_date, (int)date_get_day(birth_date))) return 1;
    if (date_set_month(user->birth_date, (int)date_get_month(birth_date))) return 1;
    if (date_set_year(user->birth_date, (int)date_get_year(birth_date))) return 1;
    return 0;
}

int set_user_country(user_t *user,char *country) {
    if(!user || !country) return 1;
    if(strlen(country) == 0) return 1;

    if(user->country){
        free(user->country);
    }
    user->country = strdup(country);
    return 0;
}

int set_user_subscription_type(user_t *user,char *subscription_type) {
    if(!subscription_type) return 1;
    if(strlen(subscription_type) == 0) return 1;

    int len = strlen(subscription_type);

    for(int i = 0; i < len; i++) subscription_type[i] = tolower(subscription_type[i]);

    if(strcmp(subscription_type,"premium") == 0) {user->subscription_type = PREMIUM;return 0;}
    if(strcmp(subscription_type,"normal") == 0) {user->subscription_type = NORMAL;return 0;}

    return 1;
}

int set_user_liked_music_ids_from_string(user_t *user, char* str) {
    if (!user || !str) return 1;
    
    for (guint i = 0; i < user->liked_music_ids->len; i++) {
        free(g_array_index(user->liked_music_ids, char*, i));
    }
    g_array_free(user->liked_music_ids, TRUE);

    
    user->liked_music_ids = string_to_array1(str);
    if(!user->liked_music_ids){
        user->liked_music_ids = g_array_new(FALSE,FALSE,sizeof(char*));
        return 1;
    }
    
    return 0;
}

int set_user_liked_music_ids(user_t *user, GArray* liked_musics) {
    if (!user) return 1;
    if (liked_musics == NULL) return 1;

    GArray* musics_list = g_array_new(FALSE, FALSE, sizeof(char*));
    for(guint i = 0; i<liked_musics->len;i++){
        char* id = g_array_index(liked_musics, char*, i);
        char* id_cpy = strdup(id);
        g_array_append_val(musics_list, id_cpy);
    }

    if(musics_list == NULL) return 1;
    
    for (guint i = 0; i < user->liked_music_ids->len; i++) {
        free(g_array_index(user->liked_music_ids, char*, i));
    }
    g_array_free(user->liked_music_ids, TRUE);

    
    user->liked_music_ids = musics_list;
    return 0;
}

guint user_get_array_size(user_t* user){
    return user->liked_music_ids->len;
}

char* get_user_name(user_t *user) {
    return strdup(user->username);
}

char* get_user_email(user_t *user) {
    return strdup(user->email);
}

char* get_user_first_name(user_t *user) {
    return strdup(user->first_name);
}

char* get_user_last_name(user_t *user) {
    return strdup(user->last_name);
}

char* get_user_birth_date_str(user_t *user) {
    char* date_str = malloc(20 * sizeof(char));
    if (date_str == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }

    int day = date_get_day(user->birth_date);
    int month = date_get_month(user->birth_date);
    int year = date_get_year(user->birth_date);

    snprintf(date_str, 20, "%04d/%02d/%02d", year, month, day);

    return date_str;
}

date_t* get_user_birth_date_struct(user_t* user){
    date_t* new_date = malloc(date_sizeof());
    if(!new_date)
        return NULL;
    date_set_day(new_date, (int)date_get_day(user->birth_date));
    date_set_month(new_date, (int)date_get_month(user->birth_date));
    date_set_year(new_date, (int)date_get_year(user->birth_date));
    return new_date;
}

char* get_user_country(user_t *user) {
    return strdup(user->country);
}

int get_user_subscription_type(user_t *user) {
    return user->subscription_type;
}

GArray* get_user_liked_music_ids(user_t *user_t) {
    GArray *music_ids_copy = g_array_new(FALSE, FALSE, sizeof(char*));
    for (guint i = 0; i < user_t->liked_music_ids->len; i++) {
        char *id = g_array_index(user_t->liked_music_ids, char*, i);
        char* id_cpy = strdup(id);
        g_array_append_val(music_ids_copy, id_cpy); 
    }
    return music_ids_copy;
}

int get_user_age(user_t* user){
    return calculate_age(user->birth_date);
}

/*GArray* get_user_history(user_t* user, pool_t* history_allocator){
    GArray *history_copy = g_array_new(FALSE, FALSE, history_sizeof());
    for (guint i = 0; i < user->user_history->len; i++) {
        history_t* history = g_array_index(user->user_history, history_t*, i);
        history_t* history_cpy = history_clone(history, history_allocator);
        g_array_append_val(history_copy, history_cpy); 
    }
    return history_copy;
}*/

GArray* get_user_history(user_t* user, pool_t* history_allocator){
    GArray *history_copy = g_array_new(FALSE, FALSE, historyPTR_sizeof());
    for (guint i = 0; i < user->user_history->len; i++) {
        history_t* history = g_array_index(user->user_history, history_t*, i);
        history_t* h_cpy = history_clone(history, history_allocator);
        g_array_append_val(history_copy, h_cpy);
    }
    return history_copy;
}

void add_user_history(user_t* user, history_t* history){
    if(user->user_history != NULL){
        g_array_append_val(user->user_history, history);
    } else {
     printf("fudeu\n");
    }
}

int create_user_from_line(user_t* user, GArray* parsed_files){
    if(!user){
        return 1;
    }
    if(parsed_files == NULL){
        return 1;
    }
    if(parsed_files->len == 0){
        return 1;
    }

    char* username = g_array_index(parsed_files, char*, 0);
    char* email = g_array_index(parsed_files, char*, 1);
    char* first_name = g_array_index(parsed_files, char*, 2);
    char* last_name = g_array_index(parsed_files, char*, 3);
    char* birth_date = g_array_index(parsed_files, char*, 4);
    char* country = g_array_index(parsed_files, char*, 5);
    char* subscription_type = g_array_index(parsed_files, char*, 6);
    char* liked_music_ids = g_array_index(parsed_files, char*, 7);

    if(set_user_name(user, username)){return 1;}
    if(set_user_email(user, email)){return 1;}
    if(set_user_first_name(user, first_name)){return 1;}
    if(set_user_last_name(user, last_name)){return 1;}
    if(set_user_birth_date_from_string(user, birth_date)){return 1;}
    if(set_user_country(user, country)){return 1;}
    if(set_user_subscription_type(user, subscription_type)){return 1;}
    if(set_user_liked_music_ids_from_string(user, liked_music_ids)){return 1;}


    

    return 0;
}

void __free_user__(void *u) {
    if (!u) return;

    user_t* user = (user_t*)u;

    free(user->username);
    free(user->email);
    free(user->first_name);
    free(user->last_name);
    free(user->birth_date);
    free(user->country);
    

    for (guint i = 0; i < user->liked_music_ids->len; i++) {
        free(g_array_index(user->liked_music_ids, char*, i));
    }
    g_array_free(user->liked_music_ids, TRUE); 

    for (guint i = 0; i < user->user_history->len; i++) {
        __free_history__(g_array_index(user->user_history, char*, i));
    }
    g_array_free(user->user_history, TRUE); 
}