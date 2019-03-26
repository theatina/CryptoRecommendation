//
//  assignment.h
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#ifndef assignment_h
#define assignment_h

#include "recom.h"
#include "cluster.h"

//Lloyd's
int closest_centroid_to_point(struct multipoints* point, struct cluster* clusters_ptr[], int clusters_num, int metric, int dimension_of_coords);
int Lloyd_s_assign(struct multipoints* MpointsList, point_num_type totalpoints_in, struct cluster* clusters_ptr[], int clusters_num, int metric, int dimension_of_coords);

//LSH-CUBE
int check_if_centroid(struct multipoints* point, struct cluster* clusters_ptr[], int clusters_num, int dimension_of_coords); //check if this point is a centroid
double min_dist_centroids(struct cluster* clusters_ptr[], int clusters_num, int metric, int dimension_of_coords);
point_num_type lock_points(struct cluster* clusters_ptr[], int clusters_num);
int assign_points_left(struct multipoints* MpointsList, point_num_type totalpointsin, struct cluster* clusters_ptr[], int clusters_num, int metric, point_num_type points_assigned, int dimension_of_coords);

//main function
void add_centroid_to_cluster(struct cluster* clusters_ptr[], int clusters_num );
//int assign_algo_assign(int assign, struct multipoints* MpointsList, point_num_type totalpoints_in, struct cluster* clusters_ptr[], int clusters_num, int hfuns_num, int hashtables_num, int metric);

#endif /* assignment_h */
