//
//  assignment.c
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#include "assignment.h"


//find the closest centroid to point so as to assign it
int closest_centroid_to_point(struct multipoints* point, struct cluster* clusters_ptr[], int clusters_num, int metric, int dimension_of_coords)
{
	int cluster_of_point = -9;
	double mindist = 0.0, tempdist = 0.0;
	//printf("\nPoint: %s\n", get_name_multipoints(point) );
	for(int y = 0; y < clusters_num; y++) //all the centroids so far
	{
		tempdist = get_dist_multipoints(get_cluster_centroid(y, clusters_ptr), point, metric, dimension_of_coords);

		if(!mindist)
		{
			mindist = tempdist;
			cluster_of_point = y;
		}
		else
		{
			if(tempdist < mindist)
			{
				mindist = tempdist;
				cluster_of_point = y;
			}
		}
		//printf("\nmindist: %lf (cluster%d)\n", mindist, cluster_of_point );
		
	}

	return cluster_of_point;
}

//assign points to their closest centroid
int Lloyd_s_assign(struct multipoints* MpointsList, point_num_type totalpoints_in, struct cluster* clusters_ptr[], int clusters_num, int metric, int dimension_of_coords)
{
	
	struct multipoints* templist = NULL;
	templist = MpointsList;
	coordtype mindist = 0.0, tempdist = 0.0;

	int cluster_of_point = -9;
	while(templist!=NULL)
	{

		int is_a_centroid = 0;

		is_a_centroid = check_if_centroid(templist, clusters_ptr, clusters_num, dimension_of_coords); 

		if(!is_a_centroid) //if not a centroid assign the point to the closest cluster - centroid
		{

			cluster_of_point = closest_centroid_to_point(templist, clusters_ptr, clusters_num, metric, dimension_of_coords);

			//printf("\nPoint to get in cluster %d: %s\n", cluster_of_point, get_name_multipoints(templist) );
			
			save_centroid_multipoints(templist, cluster_of_point);
			save_locked_multipoints(templist, 1);
			add_point_to_cluster(templist, cluster_of_point, clusters_ptr);
			

		}
		

		templist = get_next_multipoints(templist);
		cluster_of_point = -9;
		mindist = 0.0;
		tempdist = 0.0;
	}


	return 6;
}


double min_dist_centroids(struct cluster* clusters_ptr[], int clusters_num, int metric, int dimension_of_coords)
{
	struct multipoints* centroid1 = NULL, *centroid2 = NULL;
	double mindist = 0.0, tempdist = 0.0;

	for(int i = 0; i < clusters_num-1; i++)
	{	

		centroid1 = get_cluster_centroid(i, clusters_ptr);

		for (int y = i+1; y < clusters_num; y++)
		{
			centroid2 = get_cluster_centroid(y, clusters_ptr);

			tempdist = get_dist_multipoints(  centroid1, centroid2 , metric, dimension_of_coords);


			if(!mindist)
			{
				mindist = tempdist;
				//printf("\nmindist: %lf\n", mindist );
			}
			else
			{
				if( (!tempdist) &&  (tempdist < mindist))
				{
					mindist = tempdist;
					//printf("\nmindist: %lf\n", mindist );
				}
			}

		}

	}

	return mindist;
}



//LSH-CUBE

int check_if_centroid(struct multipoints* point, struct cluster* clusters_ptr[], int clusters_num, int dimension_of_coords)
{
	int is_centroid = 0;

	for(int i = 0; i < clusters_num; i++)
	{
		int same = 0;
		if((same = check_if_same(point, get_cluster_centroid(i, clusters_ptr), dimension_of_coords)) == 1)
		{
			is_centroid = 1;
			break;
		}
	}

	return is_centroid;
}

//lock the points to their clusters so as not to change at next range
point_num_type lock_points(struct cluster* clusters_ptr[], int clusters_num)
{
    point_num_type points_assigned = 0;

    for( int i = 0; i < clusters_num; i++) //for each cluster for each range I run the LSH  algorithm to find the NN-Range and save them in cluster
    {
        struct points_in_cluster* templist = NULL;

        templist = get_firstpoint_in_cluster(i, clusters_ptr);
        while(templist!=NULL)
        {
            if( !(get_locked_multipoints( get_point_points_in_cluster( templist ) ) ) )
            { 
                save_locked_multipoints( get_point_points_in_cluster( templist ) , 1 );

                points_assigned++;
            }
            templist = get_next_points_in_cluster(templist);
        }

        //printClusters(clusters_ptr, clusters_num);
    }


    return points_assigned;
}


int assign_points_left(struct multipoints* MpointsList, point_num_type totalpointsin, struct cluster* clusters_ptr[], int clusters_num, int metric, point_num_type points_assigned, int dimension_of_coords)
{
    int cluster_of_point = -9;
    struct multipoints* tempointlist = NULL;

    tempointlist = MpointsList;
    while( (points_assigned < totalpointsin) && tempointlist!=NULL)
    {

        if((get_locked_multipoints(tempointlist)) == 0)  //for the not locked points, assign them to the closest centre -> Lloyd's
        {
            cluster_of_point = closest_centroid_to_point(tempointlist, clusters_ptr, clusters_num, metric, dimension_of_coords);
            save_centroid_multipoints(tempointlist, cluster_of_point);
            save_locked_multipoints(tempointlist, 1);
            add_point_to_cluster(tempointlist, cluster_of_point, clusters_ptr);
            points_assigned++;
        }

        tempointlist = get_next_multipoints(tempointlist);
        cluster_of_point = -9;

    }

    //printf("\nFinal: points_assigned -> %lld\n", points_assigned );

    return 44;
}


void add_centroid_to_cluster(struct cluster* clusters_ptr[], int clusters_num )
{
	//add the centroid to its cluster so as to contribute to the cluster later
	for(int i = 0; i < clusters_num; i++)
	{
		if((strncmp(get_name_multipoints(get_cluster_centroid(i, clusters_ptr)), "Mean", strlen("Mean")))!= 0)
		{
			save_centroid_multipoints(get_cluster_centroid(i, clusters_ptr), i);
			save_locked_multipoints(get_cluster_centroid(i, clusters_ptr), 1);
			add_point_to_cluster(get_cluster_centroid(i, clusters_ptr), i, clusters_ptr);
		}
	}

}


//main function
// int assign_algo_assign(int assign, struct multipoints* MpointsList, point_num_type totalpoints_in, struct cluster* clusters_ptr[], int clusters_num, int hfuns_num, int hashtables_num, int metric)
// {
// 	// //add the centroid to its cluster so as to contribute to the cluster later
// 	// for(int i = 0; i < clusters_num; i++)
// 	// {
// 	// 	if((strncmp(get_name_multipoints(get_cluster_centroid(i, clusters_ptr)), "Mean", strlen("Mean")))!= 0)
// 	// 	{
// 	// 		save_centroid_multipoints(get_cluster_centroid(i, clusters_ptr), i);
// 	// 		save_locked_multipoints(get_cluster_centroid(i, clusters_ptr), 1);
// 	// 		add_point_to_cluster(get_cluster_centroid(i, clusters_ptr), i, clusters_ptr);
// 	// 	}
// 	// }

// 	if(assign == 1)
// 	{
// 		printf("\nLloyd's assignment . . .\n");
// 		Lloyd_s_assign(MpointsList, totalpoints_in, clusters_ptr, clusters_num, metric);
// 	}
// 	else if(assign == 2)
// 	{
// 		printf("\nLSH assignment . . .\n");
// 		//lsh_range(MpointsList, totalpoints_in, clusters_ptr, clusters_num, hfuns_num, hashtables_num, metric);
// 		point_num_type points_assigned = 0;
// 		points_assigned = point_assignment_lsh(MpointsList, totalpoints_in, hfuns_num, hashtables_num, tablesize, hashtablesarray, hfarray, r, metric, clusters_ptr, clusters_num );
// 		//assign points_left
// 		assign_points_left(MpointsList, totalpoints_in, clusters_ptr, clusters_num, metric, points_assigned);

// 	}
// 	else if(assign == 3)
// 	{
// 		printf("\nhyperCUBE assignment . . .\n");
// 		cube_range(MpointsList, totalpoints_in, clusters_ptr, clusters_num, hfuns_num, metric);
// 	}


// 	return 11;
// }