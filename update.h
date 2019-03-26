//
//  update.c
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#ifndef update_h
#define update_h

#include "recom.h"
#include "cluster.h"

//K-Means update
int mean_of_cluster_k(struct multipoints* tempnode, int k, struct cluster* clusters_ptr[], int dimension_of_coords);
int k_means_update(struct multipoints* MpointsList, point_num_type totalpoints_in, struct cluster* clusters_ptr[], int clusters_num, int metric, int dimension_of_coords);

//PAM update
struct multipoints* medoid_of_cluster(struct multipoints* MpointsList, point_num_type totalpoints_in, int cluster_id, struct cluster* clusters_ptr[], int metric, int dimension_of_coords);
int pam_improved_likeLloyd_s_update(struct multipoints* MpointsList, point_num_type totalpoints_in, struct cluster* clusters_ptr[], int clusters_num, int metric, int dimension_of_coords);

//main function - chooses one of the two update algorithms
int update_algo(int update, struct multipoints* MpointsList, point_num_type totalpoints_in, struct cluster* clusters_ptr[], int clusters_num, int metric, int dimension_of_coords);

#endif /* update_h */
