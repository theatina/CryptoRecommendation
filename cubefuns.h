//
//  cubefuns.h
//
//  Christina-Theano Kylafi
//  AM: 1115201200077


#ifndef cubefuns_h
#define cubefuns_h

#include "recom.h"
#include "cluster.h"

//HYPERCUBE

//mapping struct from hi(p) -> {0,1} uniformly

struct h_to_fmap;

// typedef struct h_to_fmap h_to_fmap;
// typedef struct h_to_fmap* h_to_fmap_ptr;


//struct manipulation
//get
size_t get_size_h_to_fmap();


//checking data
int check_ham_dist(int* probes, int k);

//cube functions
point_num_type ham_dist_neighbour(point_num_type probenum, int dimension, int hamdist, int pos1, int pos2);
int h_cube(point_num_type h, struct h_to_fmap* hfmap[], int n, point_num_type* mappings);
point_num_type cube_create(struct hfun* hf[], struct multipoints* point, int k, point_num_type tablesize, struct bucket* cube[], struct h_to_fmap* hfmap[], int metric, int dimension);
int assign_points_cube_algo(struct simple_list* rlist, struct cluster* clusters_ptr[], int cluster_id, int clusters_num, int metric, int dimension_of_coords);
int hypercube_algo(point_num_type totpointsin, struct multipoints* querypointnode, int k, int M, int probes, struct multipoints* points, point_num_type tablesize, struct bucket* cube[], struct hfun* hf[], struct h_to_fmap* hfmap[], point_num_type* mappings, int metric, double range, int i, int clusters_num, struct cluster* clusters_ptr[], int dimension_of_coords);

//print functions
void print_cube(point_num_type tablesize, struct bucket* table[]);
void print_h_to_fmap(struct h_to_fmap* hfmap[], int k);

//delete functions
int delete_h_to_fmap(struct h_to_fmap* list[], int k);
void delete_hfun_cube(struct hfun* list[], int k);
void delete_all_cube(struct multipoints* MpointsList, struct hfun* hfarray[], struct h_to_fmap* hfmap[], int k, struct bucket* cube[], point_num_type tablesize );

#endif /* cubefuns_h */


