//
//  initialization.c
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#include "initialization.h" 


//Random choice of centroids
int simple_rand_init(struct multipoints* MpointsList, point_num_type totalpoints_in, int clusters,  struct cluster* clusters_ptr[], int dimension_of_coords)
{
	
	point_num_type c_nums[clusters];
	memset((void*)c_nums, 0, clusters*sizeof(point_num_type));

	for (int y = 0; y<clusters; y++)
	{	
		c_nums[y] = rand() % totalpoints_in + 1;

		int counter = 0;
		while(counter < y)  //check if this centroid has already been chosen
		{
			if(c_nums[y] == c_nums[counter])
			{
				c_nums[y] = rand() % totalpoints_in + 1;
				counter = 0;
			}
			else
			{
				counter++;
			}
	
		}

		point_num_type listcounter = 1;
		struct multipoints* templist = MpointsList;
		while(listcounter < c_nums[y])
		{
			templist = get_next_multipoints(templist);
			listcounter++;
		}

		save_cluster_centroid(y, clusters_ptr, templist);
		//add_point_to_cluster(templist, y, clusters_ptr);
		save_cluster_id(y, clusters_ptr);		

	}
	
	return 8;
}


//K-Means++
int find_next_top_centroid(struct multipoints* MpointsList, struct cluster* clusters_ptr[], coordtype* partial_sums[], int position, point_num_type totalpoints_in, int dimension_of_coords)
{
	struct multipoints* templist = NULL;
	double x = 0.0;
	double tbegin = 0.0, tend = (double)(partial_sums[0][totalpoints_in - position - 1]);
	double range = (double)(tend - tbegin);
	//printf("\n\n Partialsum: %lf \n", partial_sums[0][totalpoints_in - position - 1]);
	int found = 0;
	while (!found)  
	{
		x = (double)rand()/RAND_MAX*range+tbegin; //choose a double in [ 0 ... partial_sums[n-t] ]
		//printf("\n%lf\n", x);
		point_num_type point_pos = 0;
		for (point_num_type g = 0; g <  (totalpoints_in - position); g++)  //choose the position where -> x <= partial_sums[position]
		{
			//printf("\n%lf\n", partial_sums[0][g]);
			if( partial_sums[0][g] >= x)
			{
				point_pos = g;
				break;
			}
		}

		
		point_num_type listcounter = 0;
		templist = MpointsList;
		while(listcounter < point_pos)
		{
			templist = get_next_multipoints(templist);
			listcounter++;
		}

		found = 1;
		for(int y = 0; y < position; y++) //all the centroids so far
		{
			int check = 0;
			if ((check = check_if_same(get_cluster_centroid(y, clusters_ptr), templist, dimension_of_coords) ) == 1) //check if it's already a centroid
			{
				found = 0;
				break;
			}
		}

	}

	save_cluster_centroid( position, clusters_ptr, templist);
	//add_point_to_cluster(templist, position, clusters_ptr);


	return 5;
}

coordtype find_min_dist_from_centroids(struct multipoints* MpointsList, struct cluster* clusters_ptr[], int clusters_num, int metric, int position, coordtype* partial_sums[], point_num_type totalpoints_in, int dimension_of_coords)
{
	coordtype mindist = 0.0, tempdist = 0.0;
	point_num_type counter = 0;
	coordtype maxmindist = 0.0;

	struct multipoints* templist = MpointsList;
	while ( templist != NULL ) //for every point check distance from every chosen centroid so far
	{
		int not_a_centroid = 1;
		for(int y = 0; y < position; y++) //all the centroids so far - check if it is a centroid
		{
			int check = 0;
			if ((check = check_if_same(get_cluster_centroid(y, clusters_ptr), templist, dimension_of_coords) ) == 1)
			{
				not_a_centroid = 0;
				break;
			}
		}
		
		if(not_a_centroid)  //if not a centroid
		{
			for(int y = 0; y < position; y++) //all the centroids so far - get the min distance from centroids
			{

				//printf("\n tempd: %lf\n ", tempdist);
				tempdist = get_dist_multipoints(templist, get_cluster_centroid(y, clusters_ptr), metric, dimension_of_coords);
				//if( (tempdist== 'nan') || (tempdist== '-nan') )

				if(!mindist)
				{
					mindist = tempdist;
					maxmindist = mindist;
				}
				else 
				{
					if(tempdist < mindist)
					{
						mindist = tempdist;
						
						if (mindist > maxmindist)
						{
							maxmindist = mindist;	
						}
					}
				}
			}

			//first I save the min distance and then I'll create the partial sums 
			partial_sums[0][counter] = mindist; //every non centroid point has a min distance from the centroids

			counter++;	
		}

		templist = get_next_multipoints(templist);
	}


	for(point_num_type h = 0; h < (totalpoints_in - position); h++ )
	{
		//create the min distance ^ 2
		partial_sums[0][h] = pow( ((double)partial_sums[0][h]) / ((double)maxmindist) , 2 );

		if(h > 0)
		{
			partial_sums[0][h] += partial_sums[0][h-1];  //then add the previous sum  -> [ D(1)^2 , D(1)^2+D(2)^2, ...  ]
		}
	}


	return mindist;
}

//randomly choose the first centroid for the k-means++ algorithm of initialization
int rand_choose_first_centroid(struct cluster* clusters_ptr[], point_num_type totalpoints_in, struct multipoints* MpointsList, point_num_type c_nums[])
{	

	c_nums[0] = rand() % totalpoints_in + 1;

	point_num_type listcounter = 1;
	struct multipoints* templist = MpointsList;
	while(listcounter < c_nums[0])
	{
		templist = get_next_multipoints(templist);
		listcounter++;
	}
	save_cluster_centroid(0, clusters_ptr, templist);
	save_cluster_id(0, clusters_ptr);

	return 7;
}



int k_means_plus_init(struct multipoints* MpointsList, point_num_type totalpoints_in, int clusters_num,  struct cluster* clusters_ptr[], int metric, int dimension_of_coords)
{

	int t = 0;
	point_num_type c_nums[clusters_num];
	memset((void*)c_nums, 0, clusters_num*sizeof(point_num_type));

	//randomly choose the first centroid
	//printf("\n%d\n", __LINE__);
	rand_choose_first_centroid(clusters_ptr, totalpoints_in, MpointsList, c_nums);
	//printf("\n%d\n", __LINE__);
	for (t = 1; t < clusters_num; t++)
	{
		coordtype* partial_sums[1];
		partial_sums[0] = malloc( (totalpoints_in - t) * sizeof(coordtype));
		memset((void*)partial_sums[0], 0, (totalpoints_in - t ) * sizeof(coordtype));

		//find the min distance from the centroids + create partial sums
		find_min_dist_from_centroids(MpointsList, clusters_ptr, clusters_num, metric, t, partial_sums, totalpoints_in, dimension_of_coords);

		//choose the next centroid according to the partial sums array created before
		find_next_top_centroid(MpointsList, clusters_ptr, partial_sums, t, totalpoints_in, dimension_of_coords);

		free(partial_sums[0]);
	}
	//printf("\n%d\n", __LINE__);


	return 9;
}


int init_algo(struct multipoints* MpointsList, int init, struct cluster* clusters_ptr[], point_num_type totalpoints_in, int clusters_num, int metric, int dimension_of_coords)
{
	//in case of the 12 combinations
	clear_clusters(clusters_ptr, clusters_num);

	if(init == 1)
	{	
		if(print_msgs_on == 1)
			printf("\nRandom initialization . . .\n");
		
		simple_rand_init(MpointsList, totalpoints_in, clusters_num, clusters_ptr, dimension_of_coords );
	}
	else if(init == 2)
	{
		if(print_msgs_on == 1)
			printf("\nK-means++ initialization . . .\n");
		
		k_means_plus_init(MpointsList, totalpoints_in, clusters_num, clusters_ptr, metric, dimension_of_coords);
	}


	return 10;
}


