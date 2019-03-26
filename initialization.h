//
//  initialization.h
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#ifndef initialization_h
#define initialization_h

#include "recom.h"
#include "cluster.h"

//Randomly initialize centroids
int simple_rand_init(struct multipoints* MpointsList, point_num_type totalpoints_in, int clusters, struct cluster* clusters_ptr[], int dimension_of_coords);


//K-Means++ Initialization
int find_next_top_centroid(struct multipoints* MpointsList, struct cluster* clusters_ptr[], coordtype* partial_sums[], int position, point_num_type totalpoints_in, int dimension_of_coords);
coordtype find_min_dist_from_centroids(struct multipoints* MpointsList, struct cluster* clusters_ptr[], int clusters_num, int metric, int position, coordtype* partial_sums[], point_num_type totalpoints_in, int dimension_of_coords);
int rand_choose_first_centroid(struct cluster* clusters_ptr[], point_num_type totalpoints_in, struct multipoints* MpointsList, point_num_type c_nums[]);
int k_means_plus_init(struct multipoints* MpointsList, point_num_type totalpoints_in, int clusters_num,  struct cluster* clusters_ptr[], int metric, int dimension_of_coords);


//main algorithm
int init_algo(struct multipoints* MpointsList, int init, struct cluster* clusters_ptr[], point_num_type totalpoints_in, int clusters, int metric, int dimension_of_coords);

#endif /* initialization_h */
