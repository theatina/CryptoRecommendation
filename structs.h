//
//  structs.h
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#ifndef structs_h
#define structs_h

#include "recom.h"
#include "cluster.h"

struct multipoints;
struct points_in_cluster;
struct cluster;

//functions for point manipulation/saving
struct multipoints* multiPointList_init(int dimension_of_coords);
struct multipoints* multiPointList_insertpoint(struct multipoints* multipointnode, char* name, int dimension_of_coords);
int multiPointList_insertxcoord(struct multipoints* multipointnode, coordtype xcoord, point_num_type xnum);


//struct manipulation
//get size
size_t get_size_multipoints();
size_t get_size_cluster();
size_t get_size_points_in_cluster();

//multipoints
//init
struct multipoints* newnode_multipoints(int dimension_of_coords);

//get
struct multipoints* get_point_by_name();
int get_centroid_multipoints(struct multipoints* point);
int get_locked_multipoints(struct multipoints* point);
coordtype get_coord_multipoints(struct multipoints* point, int coordnum);
coordtype* get_coord_ptr_multipoints(struct multipoints* point);
struct multipoints* get_next_multipoints(struct multipoints* point);
char* get_name_multipoints(struct multipoints* point);
int get_real_multipoints(struct multipoints* point);

//save
void save_centroid_multipoints(struct multipoints* point, int centroid);
void save_locked_multipoints(struct multipoints* point, int locked);
void save_name_multipoints(struct multipoints* point, char* name);
void save_coord_multipoints(struct multipoints* point, coordtype coord, int coordnum);
void save_next_multipoints(struct multipoints* point, struct multipoints* next);
void save_real_multipoints(struct multipoints* point, int real);

//delete
void multiPointList_delete_next_node(struct multipoints* node);

//interpoint functions
//distance
coordtype euclidean_dist(struct multipoints* point1, struct multipoints* point2, int dimension_of_coords);
coordtype euclidean_sim(struct multipoints* point1, struct multipoints* point2, int dimension_of_coords);  //formula of euclidean distance
coordtype cos_v(struct multipoints* x, struct multipoints* y, int dimension_of_coords);
coordtype cosine_dist(struct multipoints* point1, struct multipoints* point2, int dimension_of_coords); 
coordtype get_dist_multipoints(struct multipoints* point1, struct multipoints* point2, int metric, int dimension_of_coords); //get the distance between point1 and point2 depending on the metric
int check_if_same(struct multipoints* point1, struct multipoints* point2, int dimension_of_coords ); //check if point1==point2

//points_in_cluster
//init

//get
struct multipoints* get_point_points_in_cluster(struct points_in_cluster* point_in_cluster);
struct points_in_cluster* get_next_points_in_cluster(struct points_in_cluster* point_in_cluster);
//save
void save_point_points_in_cluster(struct points_in_cluster* point_in_cluster, struct multipoints* point_to_save);
void save_next_points_in_cluster(struct points_in_cluster* point, struct points_in_cluster* next);
//add
struct points_in_cluster* add_point_to_cluster(struct multipoints* point, int cluster_id, struct cluster* clusters_ptr[]);

//cluster
//init
struct cluster* clusters_init(int clusters_num, struct cluster* clusters_ptr[]);
void clear_clusters(struct cluster* clusters_ptr[], int clusters_num);
//get
point_num_type get_points_in_cluster( int cluster_id, struct cluster* cluster_ptr[] );
int get_cluster_id(int cluster_id, struct cluster* clusters_ptr[]);
struct multipoints* get_cluster_centroid(int cluster_id, struct cluster* clusters_ptr[]);
struct points_in_cluster* get_lastpoint_in_cluster(int cluster_id, struct cluster* clusters_ptr[]);
struct points_in_cluster* get_firstpoint_in_cluster(int cluster_id, struct cluster* clusters_ptr[]);

//save
void save_totpoints_in_cluster(int cluster_id, struct cluster* clusters_ptr[], point_num_type points_in_cluster);
void save_cluster_id(int cluster_id, struct cluster* clusters_ptr[]);
void save_cluster_centroid(int cluster_id, struct cluster* clusters_ptr[], struct multipoints* centroid);
void save_list_of_points_first(int cluster_id, struct cluster* clusters_ptr[], struct points_in_cluster* point);
void save_list_of_points_last(int cluster_id, struct cluster* clusters_ptr[], struct points_in_cluster* point);
//delete
void delete_point_from_cluster(struct multipoints* point, int cluster_id, struct cluster* clusters_ptr[]);

//printing funs
void print_point(struct multipoints* point, int dimension_of_coords);
void printList(struct multipoints* list, int dimension_of_coords);
void printClusters(struct cluster* clusters_ptr[], int clusters_num);
void print_cluster_size(struct cluster* clusters_ptr[], int clusters_num);
void print_rlist(struct simple_list* rlist);
void print_centroids(struct cluster* clusters_ptr[], int clusters_num);

//delete functions
void delete_multipoint(struct multipoints* point);
void delete_multipointlist(struct multipoints* list);
void delete_points_in_cluster_list(struct points_in_cluster* list);
void delete_clusters_2_2(int clusters_num, struct cluster* clusters_ptr[]);
void delete_clusters(int clusters_num, struct cluster* clusters_ptr[]);
void delete_rlist(struct simple_list* rlist);
void delete_all(struct multipoints* Mpointslist, struct cluster* clusters_ptr[], int clusters_num);
#endif /* structs_h */
