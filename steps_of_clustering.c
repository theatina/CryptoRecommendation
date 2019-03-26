//
//  steps_of_clustering.c
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#include "steps_of_clustering.h"

//objective functions of update algorithms 

double dist_from_centroid(struct multipoints* point, struct cluster* clusters_ptr[], int metric, int dimension_of_coords)
{	
	double dist = 0.0;
	int cluster_id = -9;
	cluster_id = get_centroid_multipoints(point);

	dist = get_dist_multipoints(point, get_cluster_centroid(cluster_id, clusters_ptr), metric, dimension_of_coords);

	return dist;
}

double obj_fun_PAM(struct multipoints* MpointsList, struct cluster* clusters_ptr[], int clusters_num, int metric, int dimension_of_coords)
{
	double obj_funcur = 0.0;
	double mindist = 0.0;
	//double tempdist = 0.0;
	int is_centroid = 0;

	struct multipoints* templist = NULL;
	templist = MpointsList;

	//for each point if it is not a centroid I find the 
	while(templist!=NULL )
	{
		is_centroid = 0;

		is_centroid = check_if_centroid(templist, clusters_ptr, clusters_num, dimension_of_coords);
		
		if(!is_centroid) //if it's not a centroid
		{
			mindist = 0.0;

			mindist = dist_from_centroid(templist, clusters_ptr, metric, dimension_of_coords); //find the distance of the point from its cluster's centroid

			obj_funcur += mindist;
		}

		templist = get_next_multipoints(templist);
	}


	return obj_funcur; //sum of distances of all the points from their centroids

}

double obj_fun_kmeans(struct multipoints* MpointsList, struct cluster* clusters_ptr[], int clusters_num, int metric, int dimension_of_coords)
{
	double obj_funcur = 0.0;
	double mindist = 0.0;
	double tempdist = 0.0;

	struct multipoints* templist = NULL;
	templist = MpointsList;

	while(templist!=NULL )
	{

		for (int i = 0; i < clusters_num; i++)
		{
			tempdist = get_dist_multipoints(templist, get_cluster_centroid(i, clusters_ptr), metric, dimension_of_coords);

			if(!mindist)
			{
				mindist = tempdist;
			}
			else
			{
				if(tempdist < mindist)
				{
					mindist = tempdist;
				}
			}
		}

		obj_funcur += pow(mindist, 2);

		templist = get_next_multipoints(templist);
	}


	return obj_funcur;  //sum of distances ^ 2 between the points and their closest centroid

}

double calculate_obj_fun(int algo, struct multipoints* MpointsList, struct cluster* clusters_ptr[], int clusters_num, int metric , int dimension_of_coords)
{
	double obj_funcur = 0.0;

	if(algo == 1)
	{
		obj_funcur = obj_fun_kmeans(MpointsList, clusters_ptr, clusters_num, metric, dimension_of_coords);
	}
	else if(algo == 2)
	{
		obj_funcur = obj_fun_PAM(MpointsList, clusters_ptr, clusters_num, metric, dimension_of_coords);
	}


	return obj_funcur;
}


struct results* users_choice(struct multipoints* MpointsList, struct cluster* clusters_ptr[], int init, int assign, int update, char* outputfile, point_num_type totalpoints_in, int clusters_num, int hfuns_num, int hashtables_num, int metric, int complete_print, int dimension_of_coords)
{

	int tsize = strlen(" Algorithm: xx ") + 3*sizeof(int) + 1;
	char* tempstr = NULL;
    tempstr = malloc(sizeof(char) * tsize);
    sprintf(tempstr, " Algorithm: %dx%dx%d " , init, assign, update);
    
	if(print_msgs_on == 1)
	{
		print_line();
		print_msg_centre(tempstr);
		print_line();
		
		printf("\n");
	}
    

    free(tempstr);

	//keep time
	struct timespec clustering_start, clustering_end;
	double clustering_time = 0.0;

	clock_gettime(CLOCK_MONOTONIC_RAW, &clustering_start);
	
	//Initialization--------------------------------------------------------------------------------------------------------------------------------------------
	if(print_msgs_on == 1)
		printf("\n> Initialization\n");
	init_algo(MpointsList, init, clusters_ptr, totalpoints_in, clusters_num, metric, dimension_of_coords);
	//print_centroids(clusters_ptr, clusters_num);

	//Assignment - Update till none of the centroids change ----------------------------------------------------------------------------------------------------
	int changed = 1;
	int loopcounter = 0;
	double obj_fun_change = 1.0;
	double obj_funcur = 0.0, obj_funprev = 1.0;


	char* str = NULL;
	int tempsize = strlen(" ROUND  ") + sizeof(int) + 1;
	str = malloc(sizeof(char) * tempsize );
	
	int small_obj_change = 0;
	int loop_flag = 0;
	int loops_till_the_end = loops_till_the_end_all;

	//LSH - CUBE
	int r[hfuns_num]; memset((void*)r, 0, hfuns_num*sizeof(int));
	struct hfun *hfarray[hashtables_num][hfuns_num];
	struct bucket *hashtablesarray[hashtables_num];
	point_num_type tablesize = 0;

	struct hfun *hfarray_cube[hfuns_num];
	long long mappings = 0;
	struct bucket *cube[1];	
    struct h_to_fmap *hfmap[hfuns_num]; 
    int M = 0;
    int probes = 0;


	if(assign == 2 || assign == 3)
	{
		//loop_flag = 1;
		loops_till_the_end = loops_till_the_end_range;

		if(assign == 2)//LSH
		{
			//create hashTables -  setting up LSH
			lsh_range(MpointsList, totalpoints_in, hfuns_num, hashtables_num, metric, r, hfarray, hashtablesarray, &tablesize, dimension_of_coords);
		}
		else if(assign == 3)//hyperCUBE
		{
			//create cube
			cube_range(MpointsList, totalpoints_in, clusters_ptr, clusters_num, hfuns_num, metric, &M, &probes, hfarray_cube, cube, hfmap, &tablesize, dimension_of_coords);

		}

	}

	while ( changed && (loopcounter < loops_till_the_end) )
	{	
		sprintf(str, " ROUND %d " , loopcounter);
		if(print_msgs_on == 1)
		{	print_msg_centre_line(str);
			printf("\n");
		}


		//Making sure clusters are empty before asssigning - reassigning
		for(int y = 0; y < clusters_num; y++)
		{
			save_totpoints_in_cluster(y, clusters_ptr, 0);

			delete_points_in_cluster_list(get_firstpoint_in_cluster(y, clusters_ptr));
			save_list_of_points_first(y, clusters_ptr, NULL);
			save_list_of_points_last(y, clusters_ptr, NULL);

		}

		//init multipoint flags for centroid
		struct multipoints* tttlist = NULL;
		tttlist = MpointsList;
		while(tttlist!=NULL)
		{
			save_centroid_multipoints(tttlist, -9);
			save_locked_multipoints(tttlist, 0);

			tttlist = get_next_multipoints(tttlist);
		}

		//Assign
		if(print_msgs_on == 1)
		{
			if(!loopcounter)
			{
				printf("\n> Assignment\n");
			}
			else
			{	
				printf("\n> Re-Assignment\n");
			}
		}

		//assign_algo_assign(assign, MpointsList, totalpoints_in, clusters_ptr, clusters_num, hfuns_num, hashtables_num, metric);
		add_centroid_to_cluster(clusters_ptr, clusters_num);

		//choose assignment algorithm
		if(assign == 1)
		{	
			if(print_msgs_on == 1)
				printf("\nLloyd's assignment . . .\n");
			
			Lloyd_s_assign(MpointsList, totalpoints_in, clusters_ptr, clusters_num, metric, dimension_of_coords);
		}
		else if(assign == 2)
		{
			if(print_msgs_on == 1)
				printf("\nLSH assignment . . .\n");
			//print_hashtable(tablesize, hashtables_num, hashtablesarray);
			point_num_type points_assigned = 0;
			points_assigned = point_assignment_lsh(MpointsList, totalpoints_in, hfuns_num, hashtables_num, tablesize, hashtablesarray, hfarray, r, metric, clusters_ptr, clusters_num , dimension_of_coords);
			//assign points_left
			assign_points_left(MpointsList, totalpoints_in, clusters_ptr, clusters_num, metric, points_assigned, dimension_of_coords);

		}
		else if(assign == 3)
		{
			if(print_msgs_on == 1)
				printf("\nhyperCUBE assignment . . .\n");
			//cube_range(MpointsList, totalpoints_in, clusters_ptr, clusters_num, hfuns_num, metric);
			point_num_type points_assigned = 0;
    		points_assigned = point_assignment_cube(MpointsList, totalpoints_in, hfuns_num, M, probes, tablesize, cube, hfarray_cube, hfmap,  metric, clusters_ptr, clusters_num , &mappings, dimension_of_coords);
			//assign points left
	    	assign_points_left(MpointsList, totalpoints_in, clusters_ptr, clusters_num, metric, points_assigned, dimension_of_coords);

		}



		//Update -- it will also check if the centroids have changed----------------------------------------------------------------------------------------------------------------
		
		//1 if not changed - end, 0 if changed - continues
		if(print_msgs_on == 1)
			printf("\n> Update\n");
		
		changed = update_algo(update, MpointsList, totalpoints_in, clusters_ptr, clusters_num, metric, dimension_of_coords);
		//print_cluster_size(clusters_ptr, clusters_num);
		//print_centroids(clusters_ptr, clusters_num);

		obj_funcur = calculate_obj_fun(update , MpointsList, clusters_ptr, clusters_num, metric, dimension_of_coords);
		if(loopcounter > 0)
		{
			obj_fun_change = (double)(fabs(obj_funprev - obj_funcur)) / ((double)obj_funprev);
			//printf("\nobjective change: %lf\n", obj_fun_change );
			if(obj_fun_change <= ((double)((0.001) * objective_fun_change_factor )) )//&& (obj_fun_change >= 0) )
			{
				small_obj_change = 1;
			} 

			if( (!loop_flag) && (loopcounter >= ceil((double)loops_till_the_end/(double)2)) )
			{
				loop_flag = 1;
			}

			if( (small_obj_change > 0) && (loop_flag) )
			{
				break;
			}
			
		}

		obj_funprev = obj_funcur;

		loopcounter++;
 	}
	clock_gettime(CLOCK_MONOTONIC_RAW, &clustering_end);

	if(assign == 2)
	{
		//free memory of LSH
		delete_all_lsh(hfuns_num, hashtables_num, hfarray, tablesize, hashtablesarray, metric);
	}
	else if (assign == 3)
	{
		//free memory of CUBE
		delete_all_cube(MpointsList, hfarray_cube, hfmap, hfuns_num, cube, tablesize);	
	}

 	
 	//convert to msecs
	clustering_time = (double)((clustering_end.tv_sec - clustering_start.tv_sec) * 1000000 + (clustering_end.tv_nsec - clustering_start.tv_nsec) / 1000);
	clustering_time = (double)clustering_time/1000;

 	free(str);
	if(print_msgs_on == 1)
	{		
		print_msg_centre_line("END OF ROUNDS");
		if(loopcounter >= loops_till_the_end)
		{
			print_msg_centre("( reached max loops )");
		}
		else if (small_obj_change)
		{
			print_msg_centre("( small change in objective function )");
		} 	
		printf("\n");
		printf("\n> Clustering Time: %.5fmsec\n", clustering_time);
	}

 	
	//Silhouette - Evaluation
	double silhouette_array[clusters_num + 1];
	memset((void*)silhouette_array, 0, (clusters_num + 1) * sizeof(double));
	
	//print_silhouette(silhouette_array, clusters_num);
	//printf("\n> Silhouetting");
	//silhouette(MpointsList, totalpoints_in, clusters_ptr, clusters_num, silhouette_array, metric);
	
	// int testin = 0;
	// if((testin = print_to_outputfile(init,assign, update, outputfile, MpointsList, totalpoints_in, clusters_ptr, clusters_num, metric, silhouette_array, clustering_time, complete_print )) == -99)
	// {
	// 	printf("\nSomething went wrong with the outputfile - \"print_to_outputfile\"\n");
	// 	return NULL;
	// }
	// printf("\nSilhouette: %lf \n", silhouette_array[clusters_num ] );

	//save to results for later
	// tsize = strlen("xx") + 3*sizeof(int) + 1;
    // tempstr = malloc(sizeof(char) * tsize);
    // sprintf(tempstr, "%dx%dx%d" , init, assign, update);

	struct results* tempnode = NULL;
	tempnode = results_new();
	// save_algo_results(tempnode, tempstr);
	// save_silhouette_results(tempnode, silhouette_array[clusters_num]);
	// save_clusterin_time_results(tempnode, clustering_time);

	//free(tempstr);

	return tempnode;


}


void all_combinations(struct multipoints* MpointsList, struct cluster* clusters_ptr[], char* outputfile, int clusters_num, point_num_type totalpoints_in, int hfuns_num, int hashtables_num, int metric, int complete_print, int dimension_of_coords)
{	
	// double best_silhouette = 0.0, temp_silhouette = 0.0;
	// char* tempstr = NULL;
	// int size = strlen("xxx") + 3*sizeof(int) + 1;
	// tempstr = malloc(size*sizeof(char));
	struct results* compare_results[12];


	for(int init = 1; init < 3; init++) //initialization
	{
		for(int assign = 1; assign < 4; assign++)
		{
			for(int update = 1; update < 3; update++)
			{
				int pos = ((init-1)*6 + (assign-1)*2 + (update-1)*1 ) ;
				
				compare_results[pos] = users_choice(MpointsList, clusters_ptr, init, assign, update, outputfile, totalpoints_in, clusters_num, hfuns_num, hashtables_num, metric, complete_print, dimension_of_coords);
	
				// if( (init == 1) && (assign == 1) && (update == 1) )
				// {

				// 	best_silhouette = temp_silhouette;
				// 	sprintf(tempstr, "%dx%dx%d", init, assign, update);
				// }
				// else
				// {
				// 	if(temp_silhouette > best_silhouette)
				// 	{
				// 		best_silhouette = temp_silhouette;
				// 		sprintf(tempstr, "%dx%dx%d", init, assign, update);
				// 	}
				// }

				//print_results_position(compare_results, pos);

			}
		}
	}


	print_stats(compare_results, 12, clusters_num, hfuns_num, hashtables_num, metric);

	//printf("\nBest Silhouette: Algorithm %s\n", tempstr );

	//free(tempstr);


}






