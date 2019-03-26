//
//  cube.c
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

//#include "cluster.h"
//#include "cubefuns.h"

#include "cube.h"


point_num_type point_assignment_cube( struct multipoints* MpointsList, point_num_type totalpointsin, int k, int M, int probes, point_num_type tablesize, struct bucket* cube[], struct hfun* hfarray[], struct h_to_fmap* hfmap[], int metric, struct cluster* clusters_ptr[], int clusters_num , point_num_type* mappings, int dimension_of_coords)
{
    struct multipoints* centroid = NULL;
    double range = 0.0, mindist = 0.0;

    mindist = min_dist_centroids(clusters_ptr, clusters_num, metric, dimension_of_coords);

    range = (double)mindist / ((double)2);
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

            clusters_with_no_new_points += hypercube_algo( totalpointsin, centroid, k, M, probes, MpointsList, tablesize, cube, hfarray, hfmap, mappings,  metric, range, i, clusters_num, clusters_ptr, dimension_of_coords);

            //printClusters(clusters_ptr, clusters_num);
            //print_cluster_size(clusters_ptr, clusters_num);
        }

        //lock all the points in cluster structure for this range
        points_assigned += lock_points(clusters_ptr, clusters_num);

        printf("\nPoints_Assigned_sofar: %lld(range: %lf) - clusters_with_no_new_points: %d\n", points_assigned, range, clusters_with_no_new_points);
        if(points_assigned == totalpointsin)
        {
            break;
        }

        //printf("\nclusters_with_no_new_points: %d(range: %lf)\n", clusters_with_no_new_points, range );
        int clusters_with_no_new_points_max = ceil( (double)clusters_num / (double)2 ) ;
        if((clusters_with_no_new_points >  clusters_with_no_new_points_max ) && (range >= range_factor * init_range ) )
        {   
            //printf("\nclusters_with_no_new_points: %d(range: %lf)\n", clusters_with_no_new_points, range );
            break;

        }

        range = 2*(double)range;

    }


    return points_assigned;
}

int cube_range(struct multipoints* MpointsList, point_num_type totalpointsin, struct cluster* clusters_ptr[], int clusters_num, int hfuns_num, int metric, int* M, int* probes,  struct hfun* hfarray[], struct bucket* cube[], struct h_to_fmap* hfmap[], point_num_type* tablesize_to_return, int dimension_of_coords)
{
    

    int k = hfuns_num;  //dimension - will be calculated later if not given
    (*M) = max_points_to_check;   
    (*probes) = probes_num;

    //seed for srand for later
    struct timeval time; 
    gettimeofday(&time,NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec / 1000));


    check_ham_dist(probes, k);

    //printList(MpointsList);
    
    //hashtables' size
    point_num_type tablesize = 0;
    tablesize = pow(2,k);   
    (*tablesize_to_return) = tablesize;   

    //printf("\n---> We will create %d H functions and a %d-dimensional hypercube (%lld probes)! <---\n", k, k, tablesize);
    
    // creates an array of H functions
    // struct hfun *hfarray[k];
    double begin = -1.0, end = 1.0;
    for (int l = 0; l < k; l++)
    {
        hfarray[l] = create_hfun(begin, end, dimension_of_coords);
    }

    //printhfuns(hfarray , k);
   
	//printf("\nTablesize:%lld\n", tablesize);
	// struct bucket *cube[1];	
    
    // struct h_to_fmap *hfmap[k]; 

    point_num_type mappings = 0;
    mappings = cube_create(hfarray, MpointsList, k, tablesize, cube, hfmap, metric, dimension_of_coords);
    
    //print checks
    //print_h_to_fmap(hfmap, k);
    //print_cube(tablesize, cube);

    // point_num_type points_assigned = 0;
    // points_assigned = point_assignment_cube(MpointsList, totalpointsin, k, M, probes, tablesize, cube, hfarray, hfmap,  metric, clusters_ptr, clusters_num , &mappings);

    // assign_points_left(MpointsList, totalpointsin, clusters_ptr, clusters_num, metric, points_assigned);

    //calculate size
    //calculate_memory_cube(k, tablesize, totalpointsin, r_neighbours, totprobes_visited, mappings);
    
    //free memory
    // delete_all_cube(MpointsList, hfarray, hfmap, k, cube, tablesize);
    

    //exiting
    //printf("\n\nHYPERCUBE OUT!\n\n");
    return 0;

}


