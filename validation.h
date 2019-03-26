//
//  validation.h
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#ifndef validation_h
#define validation_h

#include "recom.h"

struct known_ratings;
struct just_a_long_long;
struct validation_set;

//main validation algorithm
void validation_algo(struct tweets* tweets[], long long tweets_num, struct sentim_score_map* sentim_map[], long long sentim_map_size, struct crypto_coins* coins[], long long coins_size, struct users* users[], long long users_num, char* outputfile, int p_nearest);

//struct functions
//size
size_t get_size_known_ratings(void);
size_t get_size_just_a_long_long(void);
size_t get_size_validation_set(void);
//more
void add_pair_known_ratings(struct known_ratings* list[], long long user_i, int coin_y, double temp_rating, long long known_index);
void print_known_pairs(struct known_ratings* list[], long long known_pairs_num);
void copy_known_pairs_list(struct known_ratings* known_pairs[], struct known_ratings* known_pairs_copy[], long long known_pairs_num);
long long count_known_pairs(struct users* users[], long long users_num, int coins_num);
long long get_nth_pair_index_and_delete_from_known_ratings(struct known_ratings** known_list, long long n);
void print_folds(struct validation_set* parts_to_validate[], int folds_to_make);
void known_to_unknown(struct users* users[], long long users_num, struct known_ratings* known_ratings[], struct validation_set* parts_to_validate[], int fold);
void unknown_to_known(struct users* users[], long long users_num, struct known_ratings* known_pairs[], struct validation_set* parts_to_validate[], int fold);
//delete
void delete_validation_set(struct validation_set* parts_to_validate[], int folds_num);
void delete_known_ratings(struct known_ratings* known_pairs[], long long known_pairs_num);

//helpful subfunctions
long long create_known_pairs_list(struct known_ratings* list[], struct users* users[], long long users_num, int coins_num, long long known_index);
void split_set(struct known_ratings* known_pairs[], long long known_pairs_num, struct validation_set* parts_to_validate[], int parts);

//MAE
double sum_fabs_dif(struct users* users[], long long users_num, int coins_size, struct known_ratings* known_pairs[], struct validation_set* parts_to_validate[], int part);
double MAE_formula(struct users* users[], long long users_num, int coins_size, struct known_ratings* known_pairs[], struct validation_set* parts_to_validate[], int part);

//validation of the 4 algorithms
double validate_lsh_A(struct users* users[], long long users_num, struct known_ratings* known_pairs[], long long known_pairs_num, struct tweets* tweets[], long long tweets_num, struct crypto_coins* coins[], long long coins_size, char* outputfile, int p_nearest, struct validation_set* parts_to_validate[], int part);
double validate_lsh_B(struct users* users[], long long users_num, struct users* users_representatives[], long long users_representatives_num, struct known_ratings* known_pairs[], long long known_pairs_num, struct tweets* tweets[], long long tweets_num, struct crypto_coins* coins[], long long coins_size, char* outputfile, int p_nearest, struct validation_set* parts_to_validate[], int part);
double validate_clustering_A(struct users* users[], long long users_num, struct known_ratings* known_pairs[], long long known_pairs_num, struct tweets* tweets[], long long tweets_num, struct crypto_coins* coins[], long long coins_size, char* outputfile, int p_nearest, struct validation_set* parts_to_validate[], int part);
double validate_clustering_B(struct users* users[], long long users_num, struct users* users_representatives[], long long users_representatives_num, struct known_ratings* known_pairs[], long long known_pairs_num, struct tweets* tweets[], long long tweets_num, struct crypto_coins* coins[], long long coins_size, char* outputfile, int p_nearest, struct validation_set* parts_to_validate[], int part);
void validate_A(double* mae_lsh, double* mae_clustering, struct users* users[], long long users_num, struct known_ratings* known_pairs[], long long known_pairs_num, struct tweets* tweets[], long long tweets_num, struct crypto_coins* coins[], long long coins_size, char* outputfile, int p_nearest, struct validation_set* parts_to_validate[]);
void validate_B(double* mae_lsh, double* mae_clustering, struct users* users[], long long users_num, struct users* users_representatives[], long long users_representatives_num, struct known_ratings* known_pairs[], long long known_pairs_num, struct tweets* tweets[], long long tweets_num, struct crypto_coins* coins[], long long coins_size, char* outputfile, int p_nearest, struct validation_set* parts_to_validate[]);

#endif /* validation_h */