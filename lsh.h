//
//	lsh.h
//
//  Christina-Theano Kylafi
//  AM: 1115201200077


//all the necessary includes for the project_part1
#ifndef lsh_h
#define lsh_h

#include "recom.h"
#include "cluster.h"

//lsh
int lsh_p_range(struct simple_list* p_neighbs, struct multipoints* Mpointlist, point_num_type totalpointsin, int k, int L, point_num_type tablesize, struct bucket* table[], struct hfun* hfarray[][k], int r[], int metric, int dimension_of_coords, int p_nearest, struct users* users[], long long users_num, int coins_num, int prob);
void sort_rlist_p(struct simple_list* rlist, int p_nearest);

point_num_type point_assignment_lsh( struct multipoints* MpointsList, point_num_type totalpointsin, int k, int L, point_num_type tablesize, struct bucket* table[], struct hfun* hfarray[][k], int r[], int metric, struct cluster* clusters_ptr[], int clusters_num, int dimension_of_coords );

int lsh_range(struct multipoints* MpointsList, point_num_type totalpointsin, int k, int L, int metric, int r[], struct hfun* hfarray[][k], struct bucket* table[], point_num_type* tablesize, int dimension_of_coords);

#endif /* lsh_h */
