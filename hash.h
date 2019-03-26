//
//  hash.h
//
//  Christina-Theano Kylafi
//  AM: 1115201200077


#ifndef hash_h
#define hash_h

#include "recom.h"
#include "cluster.h"

//list for R-neighbours
struct simple_list;

//index of the H function in hfarray
struct hf_list;

//points hashed in the cube and the hashtables
struct points_hashed;

struct gfun;

struct bucket;

struct vector;

struct hfun;


//LSH

//struct manipulation

//simple_list
//init
struct simple_list* simple_list_new(void);
//sort
void sort_rlist_p(struct simple_list* rlist, int p_nearest);
//get
coordtype get_distance_simple_list(struct simple_list* list);
struct multipoints* get_point_simple_list(struct simple_list* list);
struct simple_list* get_next_simple_list(struct simple_list* list);
//save
void save_point_simple_list(struct simple_list* list, struct multipoints* point);
void save_next_simple_list(struct simple_list* list, struct simple_list* next);


//points_hashed
//init
void points_hashed_init_first(struct bucket* bucket_ptr[], long long bucketnum);
struct points_hashed* points_hashed_new();
//get
struct multipoints* get_point_points_hashed(struct points_hashed* pointhashed);
struct points_hashed* get_next_points_hashed(struct points_hashed* pointhashed);
char* get_gfunvector_points_hashed(struct points_hashed* point);
//save
void save_gfunvector_points_hashed(struct points_hashed* phashed, char* gfunv);
void save_point_points_hashed(struct points_hashed* point_hashed, struct multipoints* point);
void save_next_points_hashed(struct points_hashed* point_hashed, struct points_hashed* point);

//bucket
//get
struct bucket get_bucket_bucket(struct bucket* bucket_ptr[], long long bucketnum);
long long get_points_hashed_here_bucket(struct bucket* bucket_ptr[], long long bucketnum );
struct points_hashed* get_firstpoint_bucket(struct bucket* bucket_ptr[], long long bucketnum);
struct points_hashed* get_lastpoint_bucket(struct bucket* bucket_ptr[], long long bucketnum);
//save
void save_points_hashed_here_bucket(struct bucket* bucket_ptr[], long long bucketnum, long long points_hashed_here);
void save_firstpoint_bucket(struct bucket* bucket_ptr[], long long bucketnum, struct points_hashed* first);
void save_lastpoint_bucket(struct bucket* bucket_ptr[], long long bucketnum, struct points_hashed* last);


//get size
size_t get_size_bucket();
size_t get_size_simple_list();
size_t get_size_hfun();
size_t get_size_points_hashed();
size_t get_size_vector();


//hash functions - creation, hashing , etc
struct vector* create_vector(double begin, double end, int dimension_of_coords);
struct hfun* create_hfun(double begin, double end, int dimension_of_coords);
//struct gfun* create_gfunctions(int k, int L, struct hfun* hfarray[][k],  struct gfun* gfunctions[]);

long long h(struct hfun* array, struct multipoints* point, int metric, int n, int dimension_of_coords);
int hashing(int r[],int k, int L, struct hfun* hf[][k], struct multipoints* point, point_num_type tablesize, struct bucket* table[], int metric, int dimension_of_coords);
double true_dist(struct multipoints* points, struct multipoints* querypoint, int metric, int dimension_of_coords);
double cos_v(struct multipoints* x, struct multipoints* y, int dimension_of_coords);
struct simple_list* LSH_algo_all_neighbours( struct simple_list* rlist, long long totalpointsin, struct multipoints* querypointnode, int k, int L, struct multipoints* Mpointlist, point_num_type tablesize, struct bucket *table[], struct hfun* hf[][k], int r[], int metric, int dimension_of_coords);
int LSH_algo(point_num_type totpointsin, struct multipoints* querypointnode, int k, int L, struct multipoints* points, point_num_type tablesize, struct bucket* table[], struct hfun* hf[][k], int r[], int metric, double radius, int cluster_id, struct cluster* clusters_ptr[], int clusters_num, int dimension_of_coords );
//struct hfun* hfunlist(int k);

//print functions
//void print_g_funs(struct gfun *list[], int L);
void printhfuns(int k, int L, struct hfun* list[][k], int dimension_of_coords);
void print_hashtable(point_num_type tablesize, int L, struct bucket* table[]);


//delete functions
int delete_hfun(int k, int L, struct hfun* list[][k], int metric);
void delete_hfun_cube(struct hfun* list[], int k);
int delete_hflist(struct hf_list* list);
int delete_pointshashed(struct points_hashed* list);
//int delete_gfun(struct gfun *node);
int delete_hashtablebucket(struct bucket bucket);
int delete_hashtablebucket_cube(struct bucket* bucket_ptr[], long long bucketnum);
//void delete_hashtable(struct bucket ***htable, int L, point_num_type tablesize);
void delete_all_lsh(int k, int L, struct hfun* hfarray[][k], point_num_type tablesize, struct bucket* hashtablesarray[], int metric);





#endif /* hash_h */
