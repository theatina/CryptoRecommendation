//
//  extrafuns.h
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#ifndef extrafuns_h
#define extrafuns_h

#include "recom.h"
#include "cluster.h"


//extra helpful
double my_log( int base, long long x );
int str_isdigit(const char *str); //if it's digit it returns 1

//print - design functions
void print_line(void);
void print_msg_centre_line(char* msg);
void print_msg_centre(char* msg);
void print_welcome_msg(void);
void print_welcome_msg_recom(void);

//more funs
int choose_algos(int*, int*, int*);  //user chooses(or not) the algorithm for each step
int read_config_file(int* clusters_num, int* hfuns_num, int* hashtables_num, char* filename);
int init_params(int argc, char* argv[], int* inputflag, char* inputfile, int* configflag, char*configfile, int* outputflag, char*outputfile, int* metric, int* clusters_num, int* hfuns_num, int* hashtables_num, int* complete_print);
struct multipoints* save_points(struct multipoints* list, char *filename, point_num_type *totalpoints, int dimension_of_coords);  
int print_to_outputfile(int init, int assign, int update, char* outputfile, struct multipoints* MpointsList, point_num_type totalpoints_in, struct cluster* clusters_ptr[], int clusters_num, int metric, double silhouette_array[], double clustering_time, int complete_print );
int print_line_file(FILE* fd);

//evaluation
double dist_average(int k, struct cluster* clusters_ptr[], struct points_in_cluster* templist, int metric, int dimension_of_coords);
int second_closest_centroid(int k, struct cluster* clusters_ptr[], int clusters_num, struct points_in_cluster* templist, int metric, int dimension_of_coords);
int silhouette(struct multipoints* MpointsList, point_num_type totalpoints_in, struct cluster* clusters_ptr[], int clusters_num, double silhouette_array[], int metric, int dimension_of_coords);
void print_silhouette(double silhouette_array[], int clusters_num);

//study results of 12 combinations
struct results; 
struct results* results_new(void);
void save_algo_results(struct results* tempnode, const char* algo_choice);
void save_silhouette_results(struct results* tempnode, double value);
void save_clusterin_time_results(struct results* tempnode, double clustering_time);
void print_stats(struct results* array[], int tot_algo_num, int clusters_num, int hfuns_num, int hashtables_num, int metric);	
void print_results_position(struct results* compare_results[], int pos);
void delete_results(struct results* array[], int size);

#endif /* extrafuns_h */

