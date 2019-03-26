//
//  recommend.h
//
//  Christina-Theano Kylafi
//  AM: 1115201200077


#ifndef recommend_h
#define recommend_h
 
#include "recom.h"


//cosine LSH 1_1
int cosine_LSH_recom_1_1(struct users* users[], long long users_num, char* outputfile, int dimension_of_coords, int p_nearest, struct crypto_coins* coins[], int coins_num);

//cosine LSH 1_2
int cosine_LSH_recom_1_2(struct users* users_representatives[], long long users_representatives_num, struct users* users[], long long users_num, char* outputfile, int dimension_of_coords, int p_nearest, struct crypto_coins* coins[], int coins_num);

//clustering 2_1
int clustering_recom_2_1(struct users* users[], long long users_num, char* outputfile, int dimension_of_coords, int p_nearest, struct crypto_coins* coins[], int coins_num, long long tweets_num);

//clustering 2_2
int clustering_recom_2_2(struct users* users_representatives[], long long users_representatives_num, struct users* users[], long long users_num, char* outputfile, int dimension_of_coords, int p_nearest, struct crypto_coins* coins[], int coins_num, long long tweets_num);

//useful functions
int users_to_Mpointlist(struct users* users[], long long users_num, struct multipoints* Mpointlist, int dimension_of_coords);
int update_ratings(struct multipoints* querypointnode, struct simple_list* rlist, struct multipoints* Mpointlist, struct users* users[], long long users_num, int coins_num, int p_nearest, int metric, int dimension_of_coords, int virtual_users, int virtual_flag);
void find_best(struct users* users[], long long users_num, struct crypto_coins* coins[], int coins_num, int problem);
void label_users_from_clusters(struct cluster* clusters[], int clusters_num, struct users* users[]);
struct simple_list* cluster_to_rlist(struct cluster* clusters[], int cluster_pos);


#endif /* recommend_h */