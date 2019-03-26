//
//  cubefuns.c
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

//2 - Hypercube


#include <assert.h>
#include "cubefuns.h"


struct h_to_fmap
{
    point_num_type h;
    int f;          //0 or 1
    struct h_to_fmap *next;

};

//struct manipulation

//get
size_t get_size_h_to_fmap()
{   return sizeof(struct h_to_fmap ); }


int check_ham_dist(int* probes, int k)
{

    //check if Hamming Distance is  < 2
    int Hamming_dist = -9;
    int checkhamdist1 = 0, checkhamdist2 = 0;
    checkhamdist1 = k - (*probes);

    //check_ham_dist(probes, k, &Hamming_dist);

    if (checkhamdist1 >= 0) //if i have less probes to check than dimension 
    {
        Hamming_dist = 1;
    }
    else  //if i need to check 2-distance neighbours 
    {
        int temp = (*probes) - k;
        checkhamdist2 = (k*(k-1))/2 - temp;
    
        if (checkhamdist2 >= 0)
        {
            //printf("\nProbes: %d - %d + %d (rem: %d from %d)\n", probes, k, temp, checkhamdist2, (k*(k-1))/2 );
            Hamming_dist = 2;
        }
        else
        {   

            //printf("\n\nSorry... Hamming distance must be 0, 1 or 2!\nContinuing with default values ( probes: 10, dimension: log2(#input_points) )\n");
            Hamming_dist = 1;
            (*probes) = 10;

        }
   
    }
   
    //printf("\n> Hamming Distance: %d\n", Hamming_dist);


	return -77;

}

point_num_type ham_dist_neighbour(point_num_type probenum, int dimension, int hamdist, int pos1, int pos2)
{
	point_num_type rest = 0;
	rest = probenum;
	int bin_representation[dimension];
	memset((void*)bin_representation, 0, dimension*sizeof(int));

	//convert decimal to binary
	for(int m = dimension-1; m >= 0; m--)
	{

		if (!rest)
		{
			bin_representation[m] = 0;
		}
		else
		{
			if(!(rest % 2))
			{
				bin_representation[m] = 0;

			}
			else
			{
				bin_representation[m] = 1;
			}
			rest = rest/2;
		}


	}


	if ((!bin_representation[pos1]))
	{	
		bin_representation[pos1] = 1; 
	}
	else
	{
		bin_representation[pos1] = 0; 
	}


	if (hamdist>1)
	{
		if ((!bin_representation[pos2]))
		{	
			bin_representation[pos2] = 1; 
		}
		else
		{
			bin_representation[pos2] = 0; 
		}

	}



	//convert binary to decimal
	point_num_type neighbour = 0;
	//printf("\nNew neighbour: ");
	for (int j = 0; j < dimension; j++)
	{
		neighbour += bin_representation[j] * pow(2, dimension-j-1);
		//printf("%d ", bin_representation[j] );

	}
	//printf("(%lld)\n", neighbour );

	return neighbour;

}


int h_cube(point_num_type h, struct h_to_fmap* hfmap[], int n, point_num_type* mappings)
{
	int result = 0;
	
	struct h_to_fmap* temp = NULL, *tempbefore = NULL;
	temp = hfmap[n];
	tempbefore = NULL;
	tempbefore = temp;
	//point_num_type tempvalue = 0;
	//tempvalue = temp -> h;
	//printf("\nMap: %lld\n", *mappings );
	while (temp!=NULL)
	{	
		if ((temp->f)==-9) //first time in list
		{
			
			break;
		}	
		else if (temp->h == h)
		{
			result = temp -> f;
			return result;
		}
		
		tempbefore = temp;
		temp = temp -> next;
		//tempvalue = temp -> h;
	}
	(*mappings)++;

	//convert the value to 0 or 1
	int randcoin = 0;
	randcoin = rand() % 2;
	if(randcoin == 1)
	{
        result = 1;
    }
    else if(randcoin == 0)
    {
        result = 0;
    }

    if ((tempbefore->f)==-9) //first node - just fill the h and f members of the struct 
    {

    	tempbefore -> h = h;
    	tempbefore -> f = result;

    }
    else  //insert the new hashing to the list 
    {
    	struct h_to_fmap* newnode = malloc(sizeof(struct h_to_fmap));
    	newnode -> h = h;
    	newnode -> f = result;
    	newnode -> next = NULL;
    	tempbefore -> next = newnode;
    }


	return result;

}

point_num_type cube_create(struct hfun* hf[], struct multipoints* point, int k, point_num_type tablesize, struct bucket* cube[], struct h_to_fmap* hfmap[], int metric, int dimension_of_coords)
{

	//helpful variables
	struct multipoints* tpointslist = NULL;
	tpointslist =  point;
	point_num_type hashresult = 0;
	point_num_type h_point = 0;
	point_num_type mappings = 0;
    size_t bucket_size = get_size_bucket();

    cube[0] = malloc( bucket_size * tablesize);
    
    //printf("\nTablesize:%lld\n", tablesize);
    for (point_num_type u = 0; u < tablesize; u++)
    {
        //printf("\nhashtable[%d][%lld]\n",p, u);
        // cube[0][u].points_hashed_here = 0;
        save_points_hashed_here_bucket( cube, u, 0);
        //cube[0][u].firstpoint = NULL;
        save_firstpoint_bucket(cube, u, NULL);
        //cube[0][u].lastpoint = NULL;
        save_lastpoint_bucket(cube, u, NULL);
    }

    for (int counter = 0; counter < k; counter++)
    {
        hfmap[counter] = malloc (sizeof(struct h_to_fmap));
        hfmap[counter] -> next = NULL;
        hfmap[counter] -> f = -9;
    }

	while(tpointslist!=NULL)
	{

		hashresult = 0;

		//count the different values of hi(p)
		// int valuesnum[k];
		// memset((void*)valuesnum, 0, k*sizeof(int));
		//valuesnum[n] = count_values_of_h(hf, tpointslist, metric);
		//count_values_of_h(hf, tpointslist, metric, k, valuesnum);

		for (int n = 0; n < k; n++)
		{
			//printf("Hush: %d\n", hlist -> h_index );
			
			h_point = h(hf[n], tpointslist, metric, n, dimension_of_coords);
			//printf("\nh_point%d metric: %d\n", h_point, metric );
			//printf("\nh%d[ %lld ] =", n, h_point);
			point_num_type tempmapping = 0;
			if (metric == 1)
				h_point = h_cube(h_point, hfmap, n, &tempmapping);
			//printf(" %lld\n", h_point);
			mappings += tempmapping;


		
			hashresult += h_point * pow(2,k-n-1);


		}
		//
		//printf("\nHashresult: %lld (%s)\n", hashresult, tpointslist->name );

		//printf("\n%d mod %d  = %d\n", tempsum,tablesize, hashresult );
		//printf("\nHashfunction%d(%d) = %d\n", y, tpointslist-> idnum, hashresult);
		
		if (!(get_points_hashed_here_bucket(cube, hashresult)))
		{


			//points_hashed_init_first(cube, hashresult);
            struct points_hashed* newnode = NULL;
            newnode = points_hashed_new();
            //printf("\n%s\n", get_gfunvector_points_hashed(newnode) );
            //newnode -> gfunvector = NULL;
            save_gfunvector_points_hashed(newnode, NULL);

            //newnode -> next = NULL;
            save_next_points_hashed(newnode, NULL);

            //newnode -> point = tpointslist;
            save_point_points_hashed(newnode, tpointslist);
            
            save_firstpoint_bucket(cube, hashresult, newnode);

            //save_firstpoint_points_hashed();
			//cube[0][hashresult].firstpoint -> gfunvector = NULL;
            //save_gfunvector_points_hashed(get_firstpoint_bucket(cube, hashresult), NULL);

			//cube[0][hashresult].firstpoint -> point = tpointslist;
            //save_point_points_hashed(get_firstpoint_bucket(cube, hashresult), tpointslist);

			//cube[0][hashresult].firstpoint -> next = NULL;
            //save_next_points_hashed(get_firstpoint_bucket(cube, hashresult), NULL);


			//cube[0][hashresult].firstpoint -> name = NULL;
			//table[y][hashresult] -> lastpoint = malloc(sizeof(struct points_hashed));
	   		
            //cube[0][hashresult].lastpoint = cube[0][hashresult].firstpoint;
            save_lastpoint_bucket(cube, hashresult, get_firstpoint_bucket(cube, hashresult));

            //(cube[0][hashresult].points_hashed_here)++;
            point_num_type temp_points_hashed_here = 1;//get_points_hashed_here_bucket(cube, hashresult);
            //temp_points_hashed_here++;
            save_points_hashed_here_bucket(cube, hashresult, temp_points_hashed_here);

		}
		else
		{
			//struct points_hashed* newnode = malloc(sizeof(struct points_hashed));
            struct points_hashed* newnode= NULL;
            newnode = points_hashed_new();
            //save_firstpoint_points_hashed(cube, hashresult, newnode);

            //newnode -> gfunvector = NULL;
            save_gfunvector_points_hashed(newnode, NULL);

			//newnode -> next = NULL;
            save_next_points_hashed(newnode, NULL);

			//newnode -> point = tpointslist;
            save_point_points_hashed(newnode, tpointslist);
			
			//cube[0][hashresult].lastpoint -> next = newnode;
            save_next_points_hashed(get_lastpoint_bucket(cube, hashresult), newnode);

			//cube[0][hashresult].lastpoint  = newnode;
            save_lastpoint_bucket(cube, hashresult, newnode);
	
			//(cube[0][hashresult].points_hashed_here)++;
            point_num_type temp_points_hashed_here = get_points_hashed_here_bucket(cube, hashresult);
            temp_points_hashed_here++;
            save_points_hashed_here_bucket(cube, hashresult, temp_points_hashed_here);
		}


	
	//tpointslist = tpointslist -> next;
    tpointslist =  get_next_multipoints(tpointslist);

	}

	return mappings;

}



int assign_points_cube_algo(struct simple_list* rlist, struct cluster* clusters_ptr[], int cluster_id, int clusters_num, int metric, int dimension_of_coords)
{
    int no_new_points = 1;
    struct simple_list* temprlist = NULL, *destroy_rlist = NULL;
    temprlist = rlist;

    while (temprlist!=NULL)
    {
        //(*tot_r_neighbours)++;
        
        int check = 0;
        check = check_if_centroid(get_point_simple_list(temprlist), clusters_ptr, clusters_num, dimension_of_coords);
        //check_if_same(get_point_simple_list(temprlist), get_cluster_centroid(cluster_id, clusters_ptr));
        if(!check)
        {   
           
            //printf("\nNeighbour -> %s", get_name_multipoints(temprlist -> point ) );
            if(!(cluster_id)) //first cluster all in the structure without checking for collisions
            {
               // printf("\nLock must be 0: \n", get_locked_multipoints(temprlist -> point));
                if(!(get_locked_multipoints(get_point_simple_list(temprlist))))
                {
                    //neighbour_tot_num++;
                    no_new_points = 0;
                    save_centroid_multipoints(get_point_simple_list(temprlist), cluster_id);
                    add_point_to_cluster(get_point_simple_list(temprlist), cluster_id, clusters_ptr);
                    
                }
            }
            else //check for collisions first
            {
                //int change_cluster = 0;
                if((get_centroid_multipoints(get_point_simple_list(temprlist))) ==  -9)
                {
                    no_new_points = 0;
                    save_centroid_multipoints(get_point_simple_list(temprlist), cluster_id);
                    add_point_to_cluster(get_point_simple_list(temprlist), cluster_id, clusters_ptr);
                    
                }
                else
                {
                    if(!(get_locked_multipoints(get_point_simple_list(temprlist))))
                    {

                    
                        double tempdistance_prev = 0.0, tempdistance_cur = 0.0;
                        tempdistance_prev = get_dist_multipoints(get_point_simple_list(temprlist), get_cluster_centroid(get_centroid_multipoints(get_point_simple_list(temprlist)), clusters_ptr), metric, dimension_of_coords);
                        tempdistance_cur = get_dist_multipoints(get_point_simple_list(temprlist), get_cluster_centroid(cluster_id, clusters_ptr), metric, dimension_of_coords);
                        if(tempdistance_cur < tempdistance_prev)
                        {
                            //change_cluster = 1;
                            //save_centroid_multipoints(temprlist -> point, -9);
                            //printf("\nTotpoints_before:%lld\n", get_points_in_cluster(get_centroid_multipoints(temprlist -> point), clusters_ptr) );
                            //print_line();
                            delete_point_from_cluster(get_point_simple_list(temprlist), get_centroid_multipoints(get_point_simple_list(temprlist)), clusters_ptr);
                            //printClusters(clusters_ptr, clusters_num);
                            //printf("\nTotpoints_after:%lld\n", get_points_in_cluster(get_centroid_multipoints(temprlist -> point), clusters_ptr) );
                            save_centroid_multipoints(get_point_simple_list(temprlist), cluster_id);
                            no_new_points = 0;
                            add_point_to_cluster(get_point_simple_list(temprlist), cluster_id, clusters_ptr);

                            
                        }

                    }
                }



            }
        }
        destroy_rlist = temprlist;
        temprlist = get_next_simple_list(temprlist);
        free(destroy_rlist);
    }

    return no_new_points;
}


int hypercube_algo(point_num_type totpointsin, struct multipoints* querypointnode, int k, int M, int probes, struct  multipoints *points, point_num_type tablesize, struct bucket *cube[], struct hfun* hf[], struct h_to_fmap* hfmap[], point_num_type* mappings, int metric, double radius, int cluster_id, int clusters_num, struct cluster* clusters_ptr[], int dimension_of_coords)
{
    point_num_type no_new_points = 1;

	
    int probes_visited = 0;
    point_num_type totprobes = 0;
    int points_checked = 0;

   // printf("\n> Applying Hypercube algorithm..\n");


    int HamDistance = 1;

	
    HamDistance = 1;
    totprobes += probes_visited;
    probes_visited = 0; //set counter to 0 for every query point
    points_checked = 0; 
    int running = 1;


	point_num_type hashresult = 0;
	point_num_type h_point = 0;

	//for Range Search - if radius was > 0
	struct simple_list* rlist = NULL;
	if (radius!=0.0)
	{	
		//fprintf(outfd, "\nR-near neighbors: " );
	}

	//for Approximate - NN

	double tsum = 0.0;
	int r_neighbours = 0;

	//printf("\nSearching for neighbours..\n");

	hashresult = 0;

	for (int n = 0; n < k; n++)
	{	
		//printf("Point%d\nHindex: %d - n %d", querypointnode -> coords[d-1], hlist->h_index, n);
		h_point = h(hf[n], querypointnode, metric, n, dimension_of_coords);

		if (metric == 1)
			h_point = h_cube(h_point, hfmap, n, mappings);

		
		hashresult += h_point * pow(2, k-n-1);	
						
	}


	//NN
	//one possible NN from each hashtable
	struct points_hashed *tempp = NULL;
	//if ((table[y][hashresult] -> points_hashed_here))
	
    //tempp = cube[0][hashresult].firstpoint;
	tempp = get_firstpoint_bucket(cube, hashresult);

    //printf("\nPoints hashed at %lld: %lld\n", hashresult, cube[0][hashresult].points_hashed_here);
	//probes_visited++;
	//for each point in this bucket, I check and save the smallest distance
	int counter = 0;
	int pos1count = 0, pos2count = 1;
	point_num_type pos = hashresult;
	while(running) //checking neighbours as well
	{	
		probes_visited++;
		//printf("\nChecking Probe_%lld ( %d so far)\n", pos,  probes_visited);
		//printf("\nPoints hashed at %lld: %lld\n", pos, cube[0][pos].points_hashed_here);

		
		while (tempp!=NULL)
		{
			//printf("\nChecking Point: %s\n", tempp->point->name);
			//printf("\nChecking Point: %s\n", tempp->point->name);
			points_checked++;
			//printf("\nChecking Point: %s ( %d so far) \n",  get_name_multipoints((get_point_points_hashed(tempp)) ), points_checked);
			if (metric == 1)
			{	tsum = 0;
				for (int o = 0; o < d; o++)
				{

					tsum += pow ( fabs((get_coord_multipoints(querypointnode, o)) - (get_coord_multipoints(get_point_points_hashed( tempp), o))), 2);
				}
				tsum = sqrt(tsum);
				//printf("\nTsum = %.4lf\n", tsum);

				
			}
			else if(metric == 2) //cosine similarity
			{


				tsum = 1 - cos_v( get_point_points_hashed( tempp ) , querypointnode, dimension_of_coords );
				//printf("\nTsum:%lf\n", tsum );

			}

			
			
			//printf("%s\n", );
			//printf("\n%s - %s\n", gfunstr, tempp-> gfunvector );

			if((radius>0) && (tsum <= radius)) //check if g's need checking as well
			{
				//printf("\nLALALAPoint: %s" , get_name_multipoints((get_point_points_hashed(tempp)) ));
				r_neighbours = 1;


				if (rlist == NULL) //first neighbour
				{

					struct simple_list* newnode = NULL;
					newnode = simple_list_new();
					// newnode -> next = NULL;
					// newnode -> point = tempp -> point;
                    save_point_simple_list(newnode, get_point_points_hashed(tempp) );

					rlist = newnode;
				}
				else //if i already have at least 1 R-neighbour in list, insert this one if not already in list
				{	

					struct simple_list* rlistcheck_keep = NULL, *rlistcheck = NULL;
					int flagrlist = 0; //check if this point is already in the r-neighbours list
					rlistcheck_keep = rlist;
					rlistcheck = rlist;
					while (rlistcheck!= NULL)
					{

						if (!strcmp( get_name_multipoints( get_point_points_hashed( tempp )), get_name_multipoints( get_point_simple_list( rlistcheck )) ) )
						{
							flagrlist = 1;
							break;
						}
						

						rlistcheck_keep = rlistcheck;
						rlistcheck = get_next_simple_list(rlistcheck);

					}

					if (!flagrlist) //if this point is not yet in the list, insert it
					{	

						struct simple_list* newnode = NULL;
						newnode = simple_list_new(); 
                        //newnode -> next = NULL;
                        save_next_simple_list(newnode, NULL);
						
						//newnode -> point = tempp -> point;
                        save_point_simple_list(newnode, get_point_points_hashed(tempp));
						
                        //rlistcheck_keep -> next = newnode;
                        save_next_simple_list(rlistcheck_keep, newnode);

					}

					//r_neighbours = 1;
				}

			}
		

			counter++;
			if(points_checked < M)
			{
				//tempp = tempp -> next;
                tempp = get_next_points_hashed(tempp);
			}
			else
			{
				tempp = NULL;
			}

		}

		if((points_checked < M) && (probes_visited < probes))//check neighbours
		{
			//tempp = NULL; //in case it doesn't find a neighbour
			
			
			int found = 0;



			int endflag = 0;
			while(/*(pos1count < k) && (pos2count < k-1) */ !endflag && !found)
			{
				//print checks
				//printf("\nPoints checked: %d | Probes visited: %d\n", points_checked, probes_visited);					
				//printf("\nprobes visited: %d | k: %d | found: %d | endflag: %d\n", probes_visited, k , found, endflag);
				

				//if we have visited k probes, then we have to check for neighbours with distance
				if (probes_visited == k)
				{
					HamDistance = 2;
					pos1count = 0;
					pos2count = pos1count + 1;
				}
				
				if(HamDistance == 1)
				{
					//
					if (pos1count < k)  
					{
						//printf("\npos1count: %d | pos2count: %d\n", pos1count, pos2count );
						pos = ham_dist_neighbour(hashresult, k, HamDistance, pos1count, pos2count);
						//printf("\nNew possible probe: %lld, points hashed: %lld\n", pos, cube[0][pos].points_hashed_here);
						
						pos1count++;

						if ((get_points_hashed_here_bucket(cube, pos)) > 0)
						{
							found = 1;
						}
					}
					else
					{
						endflag = 1;
					}

				}
				else
				{
					if ( pos1count < k-1 )
					{	
						//printf("\npos1count: %d | pos2count: %d\n", pos1count, pos2count );
						pos = ham_dist_neighbour(hashresult, k, HamDistance, pos1count, pos2count);
						//printf("\nNew possible probe: %lld, points hashed: %lld\n", pos, cube[0][pos].points_hashed_here);
						if((pos1count < k-2) && (pos2count > k-2) )
						{
							pos1count++;
							pos2count = pos1count + 1;
						}
						else //if((pos1count < k) && (pos2count < k) )
						{
							pos2count++;
						}

						if (( get_points_hashed_here_bucket(cube, pos) ) > 0)
						{
							found = 1;
						}
					}
					else
					{
						endflag = 1;
					}
				}

				// if ((cube[0][pos].points_hashed_here) > 0)
				// {
				// 	found = 1;
				// }

			}
			//running = 0;

			if (found == 1)	
			{	
				//printf("\n(SUCCESS)New probe: %lld, points hashed: %lld\n", pos, cube[0][pos].points_hashed_here);

				//hashresult = pos;

				//tempp = cube[0][pos].firstpoint;
                tempp = get_firstpoint_bucket(cube, pos);

				//printf("tempp: %lld\n", pos );
			}
			else if ((pos1count > k-1) || (pos2count > k-1))
			{
				tempp = NULL;
				running = 0;

			}	

		}
		else
		{
			running = 0;
		}	

	}

	//printf("\nPoints checked: %d | Probes visited: %d\n", points_checked, probes_visited);

	//printf("Min dist: %.4lf, point id: %d\n", NNmindist, NNid );
	
	
    no_new_points = assign_points_cube_algo(rlist, clusters_ptr, cluster_id, clusters_num, metric, dimension_of_coords );



    return no_new_points;



}


//print functions
void print_cube(point_num_type tablesize, struct bucket *table[])
{
	struct points_hashed *tempoint = NULL;
    for (point_num_type u = 0; u < tablesize; u++)
    {
        printf("\nBucket%lld: Points: ", u );
    	//tempoint = table[0][u].firstpoint;
        tempoint = get_firstpoint_bucket(table, u);
    	while (tempoint!=NULL)
    	{
    	   printf("%s ", get_name_multipoints(get_point_points_hashed(tempoint)));
    		//tempoint = tempoint -> next;
    	   tempoint = get_next_points_hashed(tempoint);
        }
    }
}

void print_h_to_fmap(struct h_to_fmap *hfmap[], int k)
{	
	struct h_to_fmap* temp = NULL;
	for (int j = 0; j < k; j++)
	{	
		temp = hfmap[j];
		printf("\nHfmap[%d]", j);
		while (temp!=NULL)
		{
			printf("\nh_to_f[%lld] = %d", temp->h, temp->f);
			temp = temp->next;
		}
	}
}

int delete_h_to_fmap(struct h_to_fmap* list[], int k)
{
	struct h_to_fmap* temp1 = NULL, *temp2 = NULL;
	for (int o = 0; o < k; o++)
	{
		temp1 = list[o];		
		while (temp1!=NULL)
		{
			temp2 = temp1;
			temp1 = temp1 -> next;
			free(temp2);
			temp2 = NULL;
			
		}
	}     
    return 4;  
}

//delete functions
// void delete_hfun_cube(struct hfun* list[], int k)
// {

// 	for (int o = 0; o < k; o++)
// 	{
// 		free(list[o]->v);             
// 		list[o]->v = NULL;
// 		free(list[o]);  
// 	}
 
// }


void delete_all_cube(struct multipoints* MpointsList, struct hfun* hfarray[], struct h_to_fmap* hfmap[], int k, struct bucket *cube[], point_num_type tablesize )
{

	//delete_multipointlist(MpointsList);

	delete_hfun_cube(hfarray, k);

	delete_h_to_fmap(hfmap, k);

	for (point_num_type u = 0; u < tablesize; u++)
	{
	    delete_hashtablebucket_cube(cube, u);  
	}

    free(cube[0] );
    cube[0] = NULL;
    
    //printf("\n\nMemory Freed\n\n");

}




