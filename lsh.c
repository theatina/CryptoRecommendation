//
//  lsh.c
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#include "lsh.h"


int lsh_p_range(struct simple_list* p_neighbs, struct multipoints* Mpointlist, point_num_type totalpointsin, int k, int L, point_num_type tablesize, struct bucket* table[], struct hfun* hfarray[][k], int r[], int metric, int dimension_of_coords, int p_nearest, struct users* users[], long long users_num, int coins_num, int prob)
{
    // if(print_msgs_on == 1)
    //     printf("\nlsh_p range made it!\n");
    //print_hashtable(tablesize, L, table);

    //p neighbours
    //struct simple_list* rlist = NULL;
    struct multipoints* templist = NULL;
    templist = Mpointlist;
    long long point_counter = 0;
    //printf("\nApplying LSH for user: ");
    while(templist!=NULL)
    {
        //printf("\nlsh to point: %s", get_name_multipoints(templist));
        struct simple_list* rlist = NULL;

        // if(print_msgs_recom_on == 1)
        //     printf("%lld  ", point_counter);

        rlist = LSH_algo_all_neighbours( rlist, totalpointsin, templist, k, L, Mpointlist, tablesize, table, hfarray, r, metric, dimension_of_coords);
        //print_rlist(rlist);

        //sort
        sort_rlist_p(rlist, p_nearest);
        //print_rlist(rlist);

        //update ratings of unrated coins
        update_ratings(templist, rlist, Mpointlist, users, users_num, coins_num, p_nearest, 2, dimension_of_coords, users_num , prob);
        
        //find best

        //rlist = NULL;
        delete_rlist(rlist);
        templist = get_next_multipoints(templist);
        point_counter++;
        
    }



    return 88;
}



point_num_type point_assignment_lsh( struct multipoints* MpointsList, point_num_type totalpointsin, int k, int L, point_num_type tablesize, struct bucket* table[], struct hfun* hfarray[][k], int r[], int metric, struct cluster* clusters_ptr[], int clusters_num , int dimension_of_coords)
{
    struct multipoints* centroid = NULL;
    double range = 0.0, mindist = 0.0;

    mindist = min_dist_centroids(clusters_ptr, clusters_num, metric, dimension_of_coords);

    range = (double)mindist / ((double)2);  //initialize range
    double init_range = range;
    int clusters_with_no_new_points = 0;
    point_num_type points_assigned = 0;

    while ( range < ( range_factor *init_range) )
    {   

        //printf("\nclusters_with_no_new_points: %d(range: %lf)\n", clusters_with_no_new_points, range );
        clusters_with_no_new_points = 0;
        for( int i = 0; i < clusters_num; i++) //for each cluster for each range I run the LSH  algorithm to find the NN-Range and save them in cluster
        {
            centroid = get_cluster_centroid(i, clusters_ptr);
            //printClusters(clusters_ptr, clusters_num);
            
            //save the clusters with no new points for later
           
        
            clusters_with_no_new_points += LSH_algo( totalpointsin, centroid, k, L, MpointsList, tablesize, table, hfarray, r, metric, range, i, clusters_ptr, clusters_num, dimension_of_coords);
            
            //printClusters(clusters_ptr, clusters_num);
            //print_cluster_size(clusters_ptr, clusters_num);
        }

        //lock all the points in cluster structure for this range - so as not to change in next range
        points_assigned += lock_points(clusters_ptr, clusters_num);

        printf("\nPoints_Assigned_sofar: %lld(range: %lf) - clusters_with_no_new_points: %d\n", points_assigned, range, clusters_with_no_new_points);
        if(points_assigned == totalpointsin)  //if all the points have been assigned, we are finished
        {
            break;
        }

        //printf("\nclusters_with_no_new_points: %d(range: %lf)\n", clusters_with_no_new_points, range );
        int clusters_with_no_new_points_max = ceil( (double)clusters_num / (double)2 ) ;
        
        //if more than half of the clusters haven't assigned a new point then exit the loop 
        if((clusters_with_no_new_points >  clusters_with_no_new_points_max ) && (range >= range_factor * init_range ) )
        {   
            //printf("\nclusters_with_no_new_points: %d(range: %lf)\n", clusters_with_no_new_points, range );
            break;

        }
        //else, double the range and search again

        range = 2*(double)range;

    }


    return points_assigned;
}


int lsh_range(struct multipoints* MpointsList, point_num_type totalpointsin, int k, int L, int metric, int r[], struct hfun* hfarray[][k], struct bucket* hashtablesarray[], point_num_type* tablesize_to_return, int dimension_of_coords)
{


    //seed for srand for later
    struct timeval time; 
    gettimeofday(&time,NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec / 1000));


    //check list
    //printList(MpointsList); 
    //printf("\n\nTotal points of input: %lld\n", totalpointsin);
    
    //hashtable's size
    point_num_type tablesize = 0;
    if (metric == 1)
    {
        tablesize = ceil((double)totalpointsin/(double)tablesize_division_factor);
    }
    else if (metric == 2 )
    {
        tablesize = pow(2,k);
    }

    (*tablesize_to_return) = tablesize;

    //printf("\n> We will create %d H functions and %d hashtables(g hashfunctions) with size: %lld\n", k, L, tablesize );
    
    // creates an array of H functions
    //struct hfun *hfarray[L][k];

    //printhfuns(k, L, hfarray );
    
    //creates L hashtables
	// struct bucket *hashtablesarray[L];	

    //creates k random int values for the formula ( to create the g functions ) 
    // int r[k]; 
    // memset((void*)r, 0, k*sizeof(int));

    hashing(r, k, L, hfarray, MpointsList, tablesize, hashtablesarray, metric, dimension_of_coords);

    //print_hashtable(tablesize, L, hashtablesarray);

    
    // point_num_type points_assigned = 0;

    // points_assigned = point_assignment_lsh(MpointsList, totalpointsin, k, L, tablesize, hashtablesarray, hfarray, r, metric, clusters_ptr, clusters_num );

    // //assign points_left
    // assign_points_left(MpointsList, totalpointsin, clusters_ptr, clusters_num, metric, points_assigned);


    // //free memory
    //delete_all_lsh(k, L, hfarray, tablesize, hashtablesarray, metric);

    //calculate size
    //calculate_memory_lsh(k, L, tablesize, totalpointsin, r_neighbours);


    //printf("\n\nLSH OUT!\n\n");
    
    return 65;

}


