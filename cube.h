//
//  cube.h
//
//  Christina-Theano Kylafi
//  AM: 1115201200077


//all the necessary includes for the project_part1
#ifndef cube_h
#define cube_h

#include "recom.h"
#include "cluster.h"

point_num_type point_assignment_cube( struct multipoints* MpointsList, point_num_type totalpointsin, int k, int M, int probes, point_num_type tablesize, struct bucket* cube[], struct hfun* hfarray[], struct h_to_fmap* hfmap[], int metric, struct cluster* clusters_ptr[], int clusters_num , point_num_type* mappings, int dimension_of_coords);

int cube_range(struct multipoints* MpointsList, point_num_type totalpointsin, struct cluster* clusters_ptr[], int clusters_num, int hfuns_num, int metric, int* M, int* probes, struct hfun* hfarray[], struct bucket* cube[], struct h_to_fmap* hfmap[], point_num_type* tablesize, int dimension_of_coords);


#endif /* cube_h */
