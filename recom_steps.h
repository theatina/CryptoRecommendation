//
//  recom_steps.h
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#ifndef recom_steps_h
#define recom_steps_h

#include "recom.h"

//main
int recom_steps(struct tweets* tweets[], long long tweets_size, struct sentim_score_map* sentim_map[], long long sentim_map_size, struct crypto_coins* coins[], long long coins_size, struct users* users[], long long users_num,  char* outputfile, int p_nearest);

//subfunctions

//steps
double calculate_sentim_score(struct tweets* tweets[], long long tweets_size, struct sentim_score_map* sentim_map[], long long sentim_map_size);

void normalise_vector_user_crypto(struct users* users[], long long users_num, int coins_num);

void calculate_average_rating(struct users* users[], long long users_num, int coins_num);

long long count_users_with_coins(struct users* users[], long long users_num, int coins_size);

void clear_users_with_no_coins(struct users* users[], long long users_num, struct users* users_with_coins[], long long users_with_coins_num, int coins_size);



#endif /* recom_steps_h */