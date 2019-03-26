//
//  update.c
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#include "update.h"

//mean point of the cluster - not in dataset
int mean_of_cluster_k(struct multipoints* tempnode, int k, struct cluster* clusters_ptr[], int dimension_of_coords)
{
	struct multipoints* templist = NULL;
	struct points_in_cluster* templistcluster = NULL;
	coordtype tempsum = 0.0, prevsum = 0.0, tempcoord = 0.0;
	point_num_type totpoints_in_cluster = 0;

	templistcluster = get_firstpoint_in_cluster(k, clusters_ptr);
	totpoints_in_cluster = get_points_in_cluster(k, clusters_ptr);
	
	while (templistcluster!=NULL)
	{
		templist = get_point_points_in_cluster(templistcluster);

		//save the coordinates of the mean point one by one - adding the coordinates of every point/totalpoints 
		for(int coordcounter = 0; coordcounter < dimension_of_coords; coordcounter++)
		{
			tempcoord = get_coord_multipoints(templist, coordcounter);

			tempsum = (double)tempcoord / ((double)totpoints_in_cluster); //just to be sure and not have big sums

			prevsum = get_coord_multipoints(tempnode, coordcounter); 
			tempsum+=prevsum;

			save_coord_multipoints(tempnode, tempsum, coordcounter);

			tempsum = 0.0;
			prevsum = 0.0;	
		}

		templistcluster = get_next_points_in_cluster(templistcluster);			
		
	}

	return 11;
}

int k_means_update(struct multipoints* MpointsList, point_num_type totalpoints_in, struct cluster* clusters_ptr[], int clusters_num, int metric, int dimension_of_coords)
{

	int changed = 0;

	for(int k = 0; k < clusters_num; k++)
	{
		//printf("\n%d\n", __LINE__);
		struct multipoints* tempnode = NULL;

		tempnode = newnode_multipoints(dimension_of_coords);
		char* name = NULL;
		int tempsize = strlen("Mean") + 1;
		name = malloc(sizeof(char) * tempsize );
		//printf("\n%d\n", __LINE__);
		strcpy(name, "Mean");
		//printf("\n%s\n", name);
		//printf("\n%d\n", __LINE__);
		save_name_multipoints(tempnode, name);
		//printf("\n%d\n", __LINE__);
		mean_of_cluster_k(tempnode, k, clusters_ptr, dimension_of_coords);
		//printf("\n%d\n", __LINE__);

		char* centroidname = get_name_multipoints(get_cluster_centroid(k, clusters_ptr));
		if(!(strncmp(centroidname, "Mean", strlen("Mean"))))
		{	
			//check if the coords are the same - same point
			int same = 1;
			// printf("\n%d\n", __LINE__);
			for (int p = 0; p < dimension_of_coords; p++)
			{
				if( get_coord_multipoints( get_cluster_centroid(k, clusters_ptr) , p) != get_coord_multipoints(tempnode, p) )
				{
					same = 0;
					break;
				}
			}

			if(!same ) //if they are not the same points, then the centroid is changed
			{	
				changed = 1;
			}

		}
		else  //if we come from initialization then it changes no matter what, because the mean point will be different
		{
			changed = 1;
			save_cluster_centroid(k, clusters_ptr, NULL);  //set the pointer to NULL 
		}
		//printf("\n%d\n", __LINE__);
		// if (changed == 1)
		// {
			//free previous node
			if((get_cluster_centroid(k, clusters_ptr))!=NULL)  
			{
				delete_multipoint(get_cluster_centroid(k, clusters_ptr)); //delete the previous mean
			}

			save_cluster_centroid(k, clusters_ptr, tempnode);  //save new mean point
			//printf("\n%d(k: %d/%d)\n", __LINE__, k, clusters_num);
		// }
		// else
		// {
			//delete_multipoint(tempnode);
		//}

	}

	return changed;
}


struct multipoints* medoid_of_cluster(struct multipoints* MpointsList, point_num_type totalpoints_in, int cluster_id, struct cluster* clusters_ptr[], int metric, int dimension_of_coords)
{	
	struct multipoints* medoid = NULL;
	struct points_in_cluster *templist = NULL, *templist2 = NULL;
	double mindist = 0.0, tempdist = 0.0;
	int same = 0;

	//save the distances to make the algorithm quicker
	double **distarray = NULL;
	point_num_type points_of_cluster = get_points_in_cluster(cluster_id, clusters_ptr);
	distarray = malloc(sizeof(double *) * points_of_cluster);
	for (point_num_type x = 0; x < points_of_cluster ; x++)
	{
		distarray[x] = malloc(sizeof(double) * (x+1));

		for (point_num_type y = 0; y <= x ; y++)
		{

			distarray[x][y] = -9;
		}


	}


	point_num_type x = 0, y = 0;
	double distance_from_array = 0.0;
	
	templist = get_firstpoint_in_cluster(cluster_id, clusters_ptr);
	while(templist!=NULL)
	{	
	
		templist2 = get_firstpoint_in_cluster(cluster_id, clusters_ptr);
		while(templist2!=NULL)
		{

			if(!(same = check_if_same(get_point_points_in_cluster(templist2), get_point_points_in_cluster(templist), dimension_of_coords)))
			{
				if(x >= y)
				{
					
					if( distarray[x][y] == -9) //if the distance hasn't been saved yet, save it
					{
						distarray[x][y] = get_dist_multipoints( get_point_points_in_cluster(templist2) , get_point_points_in_cluster(templist) , metric, dimension_of_coords);
						
					}

					distance_from_array = distarray[x][y];
				}
				else
				{
					if( distarray[y][x] == -9)
					{
						distarray[y][x] = get_dist_multipoints( get_point_points_in_cluster(templist2) , get_point_points_in_cluster(templist) , metric, dimension_of_coords);
						
					}

					distance_from_array = distarray[y][x];
				}
				
				tempdist += distance_from_array;
				
			}

			templist2 = get_next_points_in_cluster(templist2);
			y++;
			
		}

		//save the point that has the minimum sum of distance from all the points and then return it
		if(!mindist)
		{
			mindist = tempdist;
			medoid = get_point_points_in_cluster( templist );
		}
		else
		{
			if( tempdist < mindist)
			{
				mindist = tempdist;
				medoid = get_point_points_in_cluster(templist);
			}

		}

		x++;
		y = 0;
		templist = get_next_points_in_cluster(templist);
		tempdist = 0.0;


	}

	//delete struct
	for (point_num_type x = 0; x < points_of_cluster ; x++)
	{
		free(distarray[x]);
		distarray[x] = NULL;
	}

	free(distarray);
	distarray = NULL;

	return medoid;
}


int pam_improved_likeLloyd_s_update(struct multipoints* MpointsList, point_num_type totalpoints_in, struct cluster* clusters_ptr[], int clusters_num, int metric, int dimension_of_coords)
{	

	int changed = 0;
	struct multipoints* medoid = NULL;
		
	for (int k = 0; k < clusters_num; k++)
	{
		//get the medoid of the cluster as the next centroid of this cluster
		medoid = medoid_of_cluster(MpointsList, totalpoints_in, k, clusters_ptr, metric, dimension_of_coords);
		if(medoid!=NULL)
		{
			char* centroidname = get_name_multipoints(get_cluster_centroid(k, clusters_ptr));
		
			if(!(strcmp(centroidname, get_name_multipoints(medoid))))
			{	}
			else
			{
				changed = 1;
				save_cluster_centroid(k, clusters_ptr, medoid);
			}

		}
		//if it's a different point, we set it as the new centroid, otherwise we keep the same - previous one


	}


	return changed;
}

//main function
int update_algo(int update, struct multipoints* MpointsList, point_num_type totalpoints_in, struct cluster* clusters_ptr[], int clusters_num, int metric, int dimension_of_coords)
{
	int changed = 0;

	if(update == 1)
	{
		if(print_msgs_on == 1)
			printf("\nK-means update . . .\n");
		
		changed = k_means_update(MpointsList, totalpoints_in, clusters_ptr, clusters_num, metric, dimension_of_coords);
		//printf("\n%d\n", __LINE__);
	}
	else if(update == 2)
	{
		if(print_msgs_on == 1)
			printf("\nPAM (improved like Lloyd's) update . . .\n");
		
		changed = pam_improved_likeLloyd_s_update(MpointsList, totalpoints_in, clusters_ptr, clusters_num, metric, dimension_of_coords);
	}




	return changed;
}
