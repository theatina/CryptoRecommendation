//
//  cluster.h
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#ifndef cluster_h
#define cluster_h


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

//forward declaration <--
struct multipoints;
struct points_in_cluster;
struct cluster;

struct bucket;
struct hfun;
struct h_to_fmap;
struct simple_list;

struct results;

typedef double coordtype; //type of coordinates of points
typedef long long point_num_type;  //type of size of dataset

#include "recom.h"

#include "structs.h"
#include "extrafuns.h"
#include "hash.h"
#include "lsh.h"
#include "cubefuns.h"
#include "cube.h"
#include "steps_of_clustering.h"
#include "initialization.h"
#include "assignment.h"
#include "update.h"

#define d 203  //dimension of dataset
#define default_cluster_num 4
#define loops_till_the_end_all 10  //how many times we well re-assign the points to the clusters
#define loops_till_the_end_range 12
#define objective_fun_change_factor 10  //1000% - 0,001

//LSH-CUBE
#define r_max_value_euclidean 1000
#define hfuns_num_def 4
#define hashtables_num_def 8

//LSH
#define tablesize_division_factor 4  //tablesize = totalpoints / tablesize_division_factor
#define w 10
#define helpful_var 4  //w_final = w / helpful_var
#define range_factor 4  //range search factor - till what range to check ( how many times the initial range )

//hyperCUBE
#define probes_num 30
#define max_points_to_check 2000

//clustering recommendation
int clustering_recommendation(struct cluster* clusters[], struct multipoints* MpointsList, char* outputfile, int metric, int init, int assign, int update, int dimension_of_coords, long long totalpoints_in, int clusters_from_recom);

//classic clustering
int clustering(struct cluster* clusters[], char* inputfile, char* configfile, char* outputfile, int metric, int init, int assign, int update, int dimension_of_coords);


#endif /* cluster_h */




