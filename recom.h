//
//  recom.h
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#ifndef recom_h
#define recom_h


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
struct coin_ref_list;
struct crypto_coins;
struct sentim_score_map;
struct tweet_content;
struct tweets;
struct list_of_tweets;
struct users;
//struct simple_list;

// #include "structs.h"
// #include "extrafuns.h"
// #include "hash.h"
// #include "lsh.h"
// #include "cubefuns.h"
// #include "cube.h"
// #include "steps_of_clustering.h"
// #include "initialization.h"
// #include "assignment.h"
// #include "update.h"
#include "extras.h"
#include "cluster.h"
#include "recom_structs.h"
#include "recom_steps.h"
#include "recommend.h"
#include "validation.h"

//prints def
#define print_msgs_on 0     //msg printing flag for clustering
#define print_msgs_recom_on 1     //msg printing flag for recommendation

//general defs
#define alpha 15
#define crypto_num 100
#define BUFSIZE 512
#define clusters_for_representatives 40
#define  clusters_for_recom 50

//recommendation
//lsh
#define default_p_nearest 20
#define metric_lsh_def 2 //cosine
#define metric_clustering_def 1 //euclidean

//validation
#define folds 10

#endif /* recom_structs_h */

