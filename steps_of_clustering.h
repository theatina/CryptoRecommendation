//
//  steps_of_clustering.h
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#ifndef steps_of_clustering_h
#define steps_of_clustering_h

#include "recom.h"
#include "cluster.h"

//objective function
double obj_fun_PAM(struct multipoints* MpointsList, struct cluster* clusters_ptr[], int clusters_num, int metric, int dimension_of_coords);
double obj_fun_kmeans(struct multipoints* MpointsList, struct cluster* clusters_ptr[], int clusters_num, int metric, int dimension_of_coords);
double calculate_obj_fun(int algo, struct multipoints* MpointsList, struct cluster* clusters_ptr[], int clusters_num, int metric, int dimension_of_coords);

struct results* users_choice(struct multipoints* MpointsList, struct cluster* clusters_ptr[], int init, int assign, int update, char* outputfile, point_num_type totalpoints_in, int clusters_num, int hfuns_num, int hashtables_num, int metric, int complete_print, int dimension_of_coords);

void all_combinations(struct multipoints* MpointsList, struct cluster* clusters_ptr[], char* outputfile, int clusters_num, point_num_type totalpoints_in, int hfuns_num, int hashtables_num, int metric, int complete_print, int dimension_of_coords);

#endif /* steps_of_clustering_h */