//
//  recom_structs.h
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#ifndef recom_structs_h
#define recom_structs_h

#include "recom.h"


//coin_ref_list
//get
char* get_coinref_coin_ref_list(struct coin_ref_list* list);
struct coin_ref_list* get_next_coin_ref_list(struct coin_ref_list* list);
char* get_nth_coinref_coin_ref_list(struct coin_ref_list* list, long long n);
//save
void save_coinref_coin_ref_list(struct crypto_coins* coins[], long long pos, char* string);

//crypto_coins
//get
int get_tot_num_crypto_coins(struct crypto_coins* coins[], long long pos);
struct coin_ref_list* get_coinreflist_crypto_coins(struct crypto_coins* coins[], long long pos);
char* get_coinref_crypto_coins(struct crypto_coins* coins[], long long pos);
struct coin_ref_list* get_lastnode_crypto_coins(struct crypto_coins* coins[], long long pos);

//save
void save_crypto_coin(struct crypto_coins* coins[], long long pos, char* string);
void crypto_coins_create_array(struct crypto_coins* coins[], long long coins_num);

//more
int is_a_coin(char* string_to_check, struct crypto_coins* coins[], int coins_num);
char* name_of_coin_from_pos(struct crypto_coins* coins[], int coins_num, int index);
//sentim_score_map
//get
char* get_srt_sentim_score_map(struct sentim_score_map* sentimap[], long long pos);
double get_score_sentim_score_map(struct sentim_score_map* sentimap[], long long pos);
double get_word_score(struct sentim_score_map* sentim_map[], long long sentim_map_size, char* tempstr);

//save
void save_srt_sentim_score_map(struct sentim_score_map* sentimap[], long long pos, char* string);
void save_score_sentim_score_map(struct sentim_score_map* sentimap[], long long pos, double score);

//tweet_content
//get
char* get_word_tweet_content(struct tweet_content* list);
struct tweet_content* get_next_tweet_content(struct tweet_content* list);
//save
void save_word_tweet_content(struct tweets* tweets[], long long pos, char* word);

//tweets
//init
void tweets_init(struct tweets* tweets[], long long tweets_num);
//get
long long get_tot_words_tweets(struct tweets* tweets[], long long pos);
long long get_user_id_tweets(struct tweets* tweets[], long long pos);
long long get_tweet_id_tweets(struct tweets* tweets[], long long pos);
double get_tweet_score_tweets(struct tweets* tweets[], long long pos);
struct tweet_content* get_tweet_content_tweets(struct tweets* tweets[], long long pos);
struct tweet_content* get_last_word_tweets(struct tweets* tweets[], long long pos);
long long get_pos_from_id_tweets(struct tweets* tweets[], long long tweets_num, long long tweet_id);
//save
void save_word_tweets(struct tweets* tweets[], long long pos, char* word);
void save_user_id_tweets(struct tweets* tweets[], long long pos, long long id);
void save_tweet_id_tweets(struct tweets* tweets[], long long pos, long long id);
void save_tweet_score_tweets(struct tweets* tweets[], long long pos, double score);
//more
void init_coins_in_tweet(int coin, struct tweets* tweets[], long long pos, struct crypto_coins* coins[]);
void add_coins_in_tweet(int coin, struct tweets* tweets[], long long pos, struct crypto_coins* coins[]);

//get_size of structs
size_t get_size_coin_ref_list();
size_t get_size_crypto_coins();
size_t get_size_sentim_score_map();
size_t get_size_tweet_content();
size_t get_size_tweets();
size_t get_size_list_of_tweets();
size_t get_size_users();

//users
//init
void users_init(struct users* users[], long long users_num, int coins_size);
//get
long long get_user_id_users(struct users* users[], long long pos);
int get_empty_user_users(struct users* users[], long long pos);
double get_user_crypto_rating_users(struct users* users[], long long pos, int coin_pos);
long long get_tot_tweets_users(struct users* users[], long long users_num);
int get_tot_coins_users(struct users* users[], long long users_num);
double get_average_rating_users(struct users* users[], long long pos);
long long get_pos_from_id_users(struct users* users[], long long users_num, long long user_id);
struct coins_in_tweet* get_nth_best_5_real_users(struct users* users[], long long pos, int n);
int get_cluster_num_real_users(struct users* users[], long long pos);
struct coins_in_tweet* get_nth_best_2_represent_users(struct users* users[], long long pos, int n);
int get_cluster_num_virtual_users(struct users* users[], long long pos);
//save
void save_empty_user_users(struct users* users[], long long pos, int is_empty);
void save_average_rating_users(struct users* users[], long long pos, double rating);
void save_user_crypto_rating_users(struct users* users[], long long pos, int coin_pos, double new_rating);
void save_nth_best_5_real_users(struct users* users[], long long pos, int n, int coin_index, char* name);
void save_cluster_num_real_users(struct users* users[], long long pos, int cluster_num_real);
void save_nth_best_2_represent_users(struct users* users[], long long pos, int n, int coin_index, char* name);
void save_cluster_num_virtual_users(struct users* users[], long long pos, int cluster_num_virtual);
void save_tot_coins_users(struct users* users[], long long pos, int coins_in_user);
//copy
void copy_users(struct users* users[], long long pos1, struct users* users_with_coins[], long long pos2, int coins_num);
void user_crypto_save_users(struct users* users[], long long users_num, int dimension_of_coords);
void retrieve_saved_user_crypto_ratings_users(struct users* users[], long long users_num, int coins_size);
void clear_users(struct users* users[], long long users_num);
//create
long long create_users(struct users* users[], long long users_num, struct tweets* tweets[], long long tweets_num, struct crypto_coins* coins[], int coins_size );
long long create_represent(struct cluster* clusters[], int clusters_num, struct users* users_representatives[], struct tweets* tweets[], long long tweets_num, struct crypto_coins* coins[], int coins_size );
//coins_users
void init_coins_in_user(int coin, struct users* users[], long long pos, struct crypto_coins* coins[]);
void add_coins_in_user(int coin, struct users* users[], long long pos, struct crypto_coins* coins[]);
int check_if_known_coin(struct users* users[], long long pos, int coin_pos);
//save data in structures
int save_data_sentim_score_map(struct sentim_score_map* structure[], const char* filename);
int save_data_crypto_coins(struct crypto_coins* structure[], const char* filename);
long long save_data_tweets(struct tweets* structure[], const char* filename, struct crypto_coins* coins[], int coins_num, int* p_nearest);
//print
void print_sentim_score_map(struct sentim_score_map* sentimscore_map[], long long size);
void print_coin_ref_list(struct coin_ref_list* list);
void print_crypto_coins(struct crypto_coins* coins[], long long size);
void print_crypto_coins_array(struct crypto_coins* coins[], long long size);
void print_tweet_content(struct tweets* tweets[], long long pos);
void print_tweets(struct tweets* tweets[], long long size, struct crypto_coins* coins[]);
void print_coins_in_user(struct users* users[], long long i, struct crypto_coins* coins[],int coins_num);
void print_users(struct users* users[], long long users_num, int coins_num, struct crypto_coins* coins[]);
void print_best5_nth_user_users(struct users* users[], long long pos);
void print_best2_nth_user_users(struct users* users[], long long pos);
void write_to_outputfile_recom(char* outputfile, struct users* users[], long long users_num,  double recom_time, int problem);
void print_all_results(struct users* users[], long long users_num,  double recom_time, int problem);
// delete
struct users* delete_users(struct users* users[], long long pos, int coins_num);
void delete_sentim_node(struct sentim_score_map* sentim_score[], long long pos);
void delete_sentim_map(struct sentim_score_map* sentim_score[], long long sentim_dictionary_size);
void delete_coin_node(struct crypto_coins* coins[], int pos);
void delete_coins(struct crypto_coins* coins[], int crypto_dictionary_size);
void delete_tweet_node(struct tweets* tweets[], long long tweet_num);
void delete_tweets(struct tweets* tweets[], long long tweets_size);
void delete_data_structures(struct tweets* tweets[], long long tweets_num, struct sentim_score_map* sentim_score[], long long sentim_dictionary_size, struct crypto_coins* coins[], int crypto_dictionary_size);

#endif /* recom_steps_h */