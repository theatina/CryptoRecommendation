//
//  hash.c
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

//1 - LSH

#include "hash.h"


//list for R-neighbours
struct simple_list
{
	coordtype distance;
    struct multipoints* point;
    struct simple_list* next;
};

//index of the H function in hfarray
struct hf_list 
{
    int h_index; 
    struct hf_list *next;

};

//points hashed in the cube and the hashtables
struct points_hashed
{
    //int idnum; //id of point
    char *gfunvector;
    struct multipoints* point;
    struct points_hashed* next;
};

struct gfun
{
    int tablenum;  //hashtable number that uses this g function as its hash function
    struct hf_list *hf_index;

};

struct bucket
{

    //list with indexes of hfunctions in the hfarray array - used to make this g function
    long long points_hashed_here; 
    struct points_hashed *firstpoint; //list with the id's of the points hashed in this bucket
    struct points_hashed *lastpoint;
};


struct vector
{

    double* vcoords[1];

};

struct hfun
{

    struct vector* v;
    float t;

};


//structs manipulation

//simple_list
//init
struct simple_list* simple_list_new(void)
{   

    struct simple_list* list = NULL;
    list = malloc(get_size_simple_list());
    list -> point = NULL;
    list -> next = NULL;

    return list;

}

//sort
/* function to swap data of two nodes a and b*/
void swap(struct simple_list* a, struct simple_list* b) 
{ 
    coordtype temp = 0.0;
	struct multipoints* temp_point = NULL;

	temp = a -> distance; 
	temp_point = a -> point;

    a -> distance = b -> distance; 
	a -> point = b -> point; 

    b -> distance = temp; 
	b -> point = temp_point;
} 

void sort_rlist_p(struct simple_list* rlist, int p_nearest)
{
    struct simple_list* start = NULL;
    start = rlist;
//     void bubbleSort(struct Node *start) 
// { 
    int swapped; 
	// int i; 
    struct simple_list *ptr1 = NULL, *lptr = NULL; 
  
    /* Checking for empty list */
    if (start == NULL) 
	{
        return; 
	}
  
    do
    { 
        swapped = 0; 
        ptr1 = start; 
  
        while (ptr1->next != lptr) 
        { 
            if (ptr1 -> distance > ptr1->next -> distance) 
            {  
                swap(ptr1, ptr1 -> next); 
                swapped = 1; 
            } 
            ptr1 = ptr1->next; 
        } 
        lptr = ptr1; 
    } while (swapped); 
//} 
  
}

//get
coordtype get_distance_simple_list(struct simple_list* list)
{   return (list -> distance); }

struct multipoints* get_point_simple_list(struct simple_list* list)
{   return (list -> point); }

struct simple_list* get_next_simple_list(struct simple_list* list)
{   return (list -> next); }

//save
void save_point_simple_list(struct simple_list* list, struct multipoints* point)
{   list -> point = point; }

void save_next_simple_list(struct simple_list* list, struct simple_list* next)
{   list -> next = next; }

//points_hashed
//init
void points_hashed_init_first(struct bucket* bucket_ptr[], long long bucketnum)
{   struct points_hashed* new = NULL;
    new = malloc(get_size_points_hashed());
    bucket_ptr[0][bucketnum].firstpoint = new; 
}

struct points_hashed* points_hashed_new(void)
{  
    struct points_hashed* phashed = NULL;
    phashed = malloc(get_size_points_hashed());
    phashed -> gfunvector = NULL;
    phashed -> point = NULL;
    phashed -> next = NULL;

    return phashed;
}

//get
struct multipoints* get_point_points_hashed(struct points_hashed* pointhashed)
{   return (pointhashed -> point); }

struct points_hashed* get_next_points_hashed(struct points_hashed* pointhashed)
{   return (pointhashed -> next); }

char* get_gfunvector_points_hashed(struct points_hashed* point)
{  return (point -> gfunvector); }

//save
void save_gfunvector_points_hashed(struct points_hashed* phashed, char* gfunv)
{   phashed -> gfunvector = gfunv; }

void save_point_points_hashed(struct points_hashed* point_hashed, struct multipoints* point)
{   point_hashed -> point = point; }

void save_next_points_hashed(struct points_hashed* point_hashed, struct points_hashed* point)
{   point_hashed -> next = point; }


//bucket
//get
struct bucket get_bucket_bucket(struct bucket* bucket_ptr[], long long bucketnum)
{   return (bucket_ptr[0][bucketnum]); }

long long get_points_hashed_here_bucket(struct bucket* bucket_ptr[], long long bucketnum )
{   return (bucket_ptr[0][bucketnum].points_hashed_here); }

struct points_hashed* get_firstpoint_bucket(struct bucket* bucket_ptr[], long long bucketnum)
{   return (bucket_ptr[0][bucketnum].firstpoint); }

struct points_hashed* get_lastpoint_bucket(struct bucket* bucket_ptr[], long long bucketnum)
{   return (bucket_ptr[0][bucketnum].lastpoint); }

//save
void save_points_hashed_here_bucket(struct bucket* bucket_ptr[], long long bucketnum, long long points_hashed_here)
{   bucket_ptr[0][bucketnum].points_hashed_here = points_hashed_here; }

void save_firstpoint_bucket(struct bucket* bucket_ptr[], long long bucketnum, struct points_hashed* first)
{   bucket_ptr[0][bucketnum].firstpoint = first; }

void save_lastpoint_bucket(struct bucket* bucket_ptr[], long long bucketnum, struct points_hashed* last)
{   bucket_ptr[0][bucketnum].lastpoint = last; }


//bucket
//get
// struct bucket get_bucket_bucket(struct bucket* bucket_ptr[], long long bucketnum)
// {   return (bucket_ptr[0][bucketnum]); }

//get size
size_t get_size_bucket()
{   return sizeof(struct bucket); }

size_t get_size_simple_list()
{   return sizeof(struct simple_list); }

size_t get_size_hfun()
{   return sizeof(struct hfun); }

size_t get_size_points_hashed()
{   return sizeof(struct points_hashed); }

size_t get_size_vector()
{   return sizeof(struct vector); }


//hash functions - creation, hashing, etc -------------------------------------------------------------------------------------------------------------------------------------------------
struct vector* create_vector(double begin, double end, int dimension_of_coords)
{	


	float a = 0.0, b = 0.0, s = 0.0, X = 0.0, Y = 0.0;
	struct vector *v = NULL;
	v = malloc(sizeof(struct vector));
	v -> vcoords[0] = malloc(dimension_of_coords*sizeof(double));
	//memset((void*)v->vcoords, 0, d*sizeof(double));
	memset((void*)v->vcoords[0], 0, dimension_of_coords*sizeof(double));

	//gdouble begin = -1.0, end = 1.0;

	for (int i = 0; i < dimension_of_coords; i = i+2)
	{
		
		do
		{
			// a = (float) g_random_double_range(begin, end);
			// b = (float) g_random_double_range(begin, end);
	
			double range = (double)(end - begin); 

    		a = (double)rand()/RAND_MAX*range+begin;
    		b = (double)rand()/RAND_MAX*range+begin;

			s = pow(a,2) + pow(b,2);
		}
		while (s >= 1);

		X = a * sqrt((-2*(float)log((double)s))/s);
		Y = b * sqrt((-2*(float)log((double)s))/s);

		if (i==dimension_of_coords-1)
		{
			v->vcoords[0][i] = X;

		}
		else
		{
			v->vcoords[0][i] = X;
			v->vcoords[0][i+1] = Y;
		}
	}

	return v;
}


struct hfun* create_hfun(double begin, double end, int dimension_of_coords) //creates an H function with random v and t
{

	struct hfun *hf = NULL;
	hf = malloc(sizeof(struct hfun));

	hf->v = NULL;
	hf->t = 0;

	struct vector *v = create_vector(begin, end, dimension_of_coords);

	float t = 0.0;
	double tbegin = 0.0, tend = (double)w;
	double range = (double)(tend - tbegin);
	t = (float)rand()/RAND_MAX*range+begin;

	//t = (float) g_random_double_range(tbegin, tend);

	hf->v = v;
	hf->t = t;


	return hf;
}


//function that returns the h_n( point )
long long h(struct hfun* array, struct multipoints* point, int metric, int n, int dimension_of_coords)
{
	long long result = 0;

	if (metric == 1)
	{
		coordtype tempsum = 0.0;

		//the formula we were given for the h_n(point)
		for (int r =0; r < dimension_of_coords; r++)
		{
			tempsum +=  (double)( array -> v -> vcoords[0][r] ) * (double)( get_coord_multipoints(point, r) ) ;  
		}

        //printf("\nTempsum + t: %lf\n", tempsum + (array -> t) );
        double tempres = (double)(tempsum + (array -> t))/((double)w);
        //printf("\ntempResult: %lf\n", tempres );
		result = floor((double) tempres * (double)helpful_var);
        //printf("\nResult: %lld\n", result );
	}
	else if (metric == 2)
	{
		coordtype temp = 0.0;

		//here we have another formula that gives us the h_n(point) = 0/1 
		for (int r =0; r < dimension_of_coords; r++)
		{
			temp +=  (double)( array -> v -> vcoords[0][r] ) * (double)( get_coord_multipoints(point, r) ) ;
		}
        //printf("\nTemp: %lf\n", temp );

		if (temp>=0)
			result = 1;
		
	}
	else
	{
		printf("\nERROR in \"h\": metric is wrong!!!!\n");
	}

	return result;
}


//fills the hashTables in LSH algorithm
int hashing(int r[], int k, int L, struct hfun* hf[][k], struct multipoints* point, point_num_type tablesize, struct bucket *table[], int metric, int dimension_of_coords)
{
	/*print the r - vector*/
	// printf("\nr = [ ");
	// for (int e = 0; e < k; e++)
	// {
	// 	printf("%d ", r[e]);
	// }
	//printf("]\n");

    double begin = -1.0, end = 1.0;
    if (metric == 1)
	{    
		for (int i = 0; i < L; i++ )
	    {
	        for (int l = 0; l<k; l++)
	        {
	            hf[i][l] = create_hfun(begin, end, dimension_of_coords);
	        }
	    }
	}
	else if(metric == 2)
	{
		for (int l = 0; l<k; l++)
        {
            hf[0][l] = create_hfun(begin, end, dimension_of_coords);
            //tot_structure_size += sizeof(hfarray[l]) + sizeof(struct vector);
        }

	}

	for(int i=0; i < L; i++)
    {
		table[i] = malloc(sizeof(struct bucket)*tablesize);

	} 
	//printf("\nTablesize:%lld\n", tablesize);
    for (int p = 0; p < L; p++)
    {
        for (point_num_type u = 0; u < tablesize; u++)
        {
            //printf("\nhashtable[%d][%lld]\n",p, u);
           	table[p][u].points_hashed_here = 0;
            table[p][u].firstpoint = NULL;
            table[p][u].lastpoint = NULL;
        }
    }

	for (int e = 0; e < k; e++)
    {
        r[e] = rand() % r_max_value_euclidean + 1 ;
    }

	//helpful variables
	struct multipoints* tpointslist = NULL;
	//struct hf_list *hlist = NULL;
	
	point_num_type hashresult = 0;
	point_num_type tempsum = 0;
	long long h_point = 0;
	
	tpointslist =  point;
    point_num_type point_counter = 0;
	//for all the points in the points list, hash them with the L g functions to the L hashtables
	while(tpointslist!=NULL)
	{

		for (int y = 0; y < L; y++)
		{
			hashresult = 0;
			tempsum = 0;
			//hlist = gfuns[y] -> hf_index; //the first h of the hash function(g) of this hashtable
			if (metric == 1)
			{
				char *gfunstr = NULL;
				int strsize = k*sizeof(long long) + (k-1)*sizeof("#") + 1;
				gfunstr = malloc(strsize);

				char *tempgfunstr = NULL;
				tempgfunstr = gfunstr;

				int pluslen = 0;

				for (int n = 0; n < k; n++)
				{
					if (metric == 1)
					{
						h_point = h(hf[y][n], tpointslist, metric, n, dimension_of_coords);
					}
					else
					{
						h_point = h(hf[0][n], tpointslist, metric, n, dimension_of_coords);
					}

					if (n == 0)
					{	
						sprintf(tempgfunstr + pluslen, "%lld#", h_point);
						pluslen = strlen(tempgfunstr);
						*(gfunstr + pluslen) = '\0';
					}
					else if (n<k-1)
					{
						pluslen = strlen(gfunstr);
						sprintf(tempgfunstr + pluslen, "%lld#", h_point);
						pluslen = strlen(tempgfunstr);
						*(gfunstr + pluslen) = '\0';
					}
					else
					{	
						pluslen = strlen(gfunstr);
						sprintf(tempgfunstr + pluslen, "%lld", h_point);
						pluslen = strlen(tempgfunstr);
						*(gfunstr + pluslen) = '\0';
					}

		            //printf("\nh(%lld): %lld\n", point_counter, h_point );
					tempsum+= (((r[n] * h_point) % tablesize ) + tablesize) % tablesize;

					//hlist = hlist -> next;
				}

				*(gfunstr + strlen(gfunstr)) = '\0';
				hashresult = (((tempsum ) % tablesize)+tablesize)%tablesize;
                //printf("\nHashresult of point %lld: %lld\n", point_counter, hashresult );

				if (!(table[y][hashresult].points_hashed_here))
				{

					table[y][hashresult].firstpoint = malloc(sizeof(struct points_hashed));
					table[y][hashresult].firstpoint -> gfunvector = gfunstr;
					table[y][hashresult].firstpoint -> point = tpointslist;
					table[y][hashresult].firstpoint -> next = NULL;

					table[y][hashresult].lastpoint = table[y][hashresult].firstpoint;

					(table[y][hashresult].points_hashed_here)++;

				}
				else
				{
					struct points_hashed *newnode = malloc(sizeof(struct points_hashed));
					newnode -> next = NULL;
					newnode -> point = tpointslist;
					newnode -> gfunvector = gfunstr;
					table[y][hashresult].lastpoint -> next = newnode;
					table[y][hashresult].lastpoint  = newnode;

					
					(table[y][hashresult].points_hashed_here)++;

				}

			}
			else if(metric == 2)
			{
				for (int n = 0; n < k; n++)
				{

					h_point = h(hf[0][n], tpointslist, metric, n, dimension_of_coords);
				

					hashresult += h_point * pow(2,k-n-1);


					//hlist = hlist -> next;
				}


				if (!(table[y][hashresult].points_hashed_here))
				{

					table[y][hashresult].firstpoint = malloc(sizeof(struct points_hashed));
					table[y][hashresult].firstpoint -> gfunvector = NULL;
					table[y][hashresult].firstpoint -> point = tpointslist;
					table[y][hashresult].firstpoint -> next = NULL;

					table[y][hashresult].lastpoint = table[y][hashresult].firstpoint;

					(table[y][hashresult].points_hashed_here)++;

				}
				else
				{
					struct points_hashed *newnode = malloc(sizeof(struct points_hashed));
					newnode -> next = NULL;
					newnode -> point = tpointslist;
					newnode -> gfunvector = NULL;
					table[y][hashresult].lastpoint -> next = newnode;
					table[y][hashresult].lastpoint  = newnode;

					
					(table[y][hashresult].points_hashed_here)++;

				}
			}


		}

		tpointslist = get_next_multipoints( tpointslist );
        point_counter++;

	}


	return -7;

}




double true_dist(struct multipoints* points, struct multipoints* querypoint, int metric, int dimension_of_coords)
{	


	double dist = -9.0, tempdist = 0.0;

    struct multipoints* trueNN = NULL;

	struct multipoints* temp = NULL;
	temp = points;

	while (temp!=NULL)
	{
		if (metric == 1)
		{	
			//printf("\npoints: |%s| - |%s|", querypoint->name , temp->name);
			tempdist = 0.0;
			for (int u = 0; u < dimension_of_coords; u++)
			{
				//printf("\n | %d:  %d - %d", u, querypoint -> coords[u] , temp-> coords[u]);
                tempdist += pow ( fabs(( get_coord_multipoints(querypoint, u) ) - ( get_coord_multipoints(temp, u) )), 2);
                //printf("\nTempdist: %lf\n", tempdist );
                
            }

            //printf("\n%s\n", get_name_multipoints(querypoint));
			tempdist = sqrt(tempdist);
            //printf("\nTempdist: %lf\n", tempdist );
		}
		else if (metric == 2)
		{


			tempdist = 1 - cos_v(temp, querypoint, dimension_of_coords);
			//printf("\n%s-%sTempdist_true: %f\n", temp->name, querypoint->name, tempdist);

		}

		if (dist == -9.0)
		{
			dist = tempdist;
            trueNN = temp;
		}

		if (tempdist < dist)
		{
			dist = tempdist;
            trueNN = temp;
		}

		temp = get_next_multipoints(temp);
        //temp = temp -> next;
	}

    //printf("\nTrueNN: %s\n", get_name_multipoints(trueNN) );


	return dist;
}

int assign_points_lsh_algo(struct simple_list* rlist, struct cluster* clusters_ptr[], int cluster_id, int clusters_num, int metric, int dimension_of_coords)
{
    int no_new_points = 1;
    struct simple_list* temprlist = NULL, *destroy_rlist = NULL;
    temprlist = rlist;

    while (temprlist!=NULL)
    {
        //(*tot_r_neighbours)++;
        
        int check = 0;
        check = check_if_centroid(temprlist -> point, clusters_ptr, clusters_num, dimension_of_coords);
        //check_if_same(temprlist -> point, get_cluster_centroid(cluster_id, clusters_ptr));
        if(!check)
        {   
           
            //printf("\nNeighbour -> %s", get_name_multipoints(temprlist -> point ) );
            if(!(cluster_id)) //first cluster all in the structure without checking for collisions
            {
               // printf("\nLock must be 0: \n", get_locked_multipoints(temprlist -> point));
                if(!(get_locked_multipoints(temprlist -> point))) //in case to know if it is already locked - when we are not in the first range
                {
                    //neighbour_tot_num++;
                    no_new_points = 0;
                    save_centroid_multipoints(temprlist -> point, cluster_id);
                    add_point_to_cluster(temprlist -> point, cluster_id, clusters_ptr);
                    
                }
            }
            else //check for collisions first
            {
                if((get_centroid_multipoints(temprlist -> point)) ==  -9) //not in a cluster yet - even in this range
                {
                    no_new_points = 0;
                    save_centroid_multipoints(temprlist -> point, cluster_id);
                    add_point_to_cluster(temprlist -> point, cluster_id, clusters_ptr);
                    
                }
                else
                {
                    if(!(get_locked_multipoints(temprlist -> point))) //if it is not locked, we check the distances
                    {

                    
                        double tempdistance_prev = 0.0, tempdistance_cur = 0.0;
                        tempdistance_prev = get_dist_multipoints(temprlist -> point, get_cluster_centroid(get_centroid_multipoints(temprlist -> point), clusters_ptr), metric, dimension_of_coords);
                        tempdistance_cur = get_dist_multipoints(temprlist -> point, get_cluster_centroid(cluster_id, clusters_ptr), metric, dimension_of_coords);
                        if(tempdistance_cur < tempdistance_prev)
                        {
                            //save_centroid_multipoints(temprlist -> point, -9);
                            //printf("\nTotpoints_before:%lld\n", get_points_in_cluster(get_centroid_multipoints(temprlist -> point), clusters_ptr) );
                            delete_point_from_cluster(temprlist -> point, get_centroid_multipoints(temprlist -> point), clusters_ptr);
                            //printClusters(clusters_ptr, clusters_num);
                            //printf("\nTotpoints_after:%lld\n", get_points_in_cluster(get_centroid_multipoints(temprlist -> point), clusters_ptr) );
                            save_centroid_multipoints(temprlist -> point, cluster_id);                      
                            add_point_to_cluster(temprlist -> point, cluster_id, clusters_ptr);
                            no_new_points = 0;

                            
                        }

                    } //else, it is locked and already assigned
                }



            }
        }
        destroy_rlist = temprlist;
        temprlist = temprlist -> next;
        free(destroy_rlist);
    }

    return no_new_points;
}

struct simple_list* LSH_algo_all_neighbours( struct simple_list* rlist, long long totalpointsin, struct multipoints* querypointnode, int k, int L, struct multipoints* Mpointlist, point_num_type tablesize, struct bucket *table[], struct hfun* hf[][k], int r[], int metric, int dimension_of_coords)
{


    long long hashresult = 0;
	long long tempsum = 0;
	long long h_point = 0;

	//for Range Search - if radius was > 0
	//struct simple_list* rlist = NULL;

	double tsum = 0.0;
	int r_neighbours = 0;

	//printf("\nSearching for neighbours..\n");
    //printf("Centroid: %s " , get_name_multipoints( querypointnode) );
    for (int y = 0; y < L; y++)
	{
		hashresult = 0;

		tempsum = 0;
		//hlist = gfuns[y] -> hf_index; //the first h of the hash function(g) of this hashtable
		char *gfunstr = NULL;
		if (metric == 1)
		{
			
			int strsize = k*sizeof(long long) + (k-1)*sizeof("#") + 1;
			gfunstr = malloc(strsize);
			char *tempgfunstr = NULL;
			tempgfunstr = gfunstr;
			int pluslen = 0;

			for (int n = 0; n < k; n++)
			{	
				//printf("Point%d\nHindex: %d - n %d", querypointnode -> coords[d-1], hlist->h_index, n);
			
				h_point = h(hf[y][n], querypointnode, metric, n, dimension_of_coords);
				
				
				//h_point = h(hf[y][n], querypointnode, metric, n);


				if (n == 0)
				{	
					sprintf(tempgfunstr + pluslen, "%lld#", h_point);
					pluslen = strlen(tempgfunstr);
					*(gfunstr + pluslen) = '\0';
				}
				else if (n<k-1)
				{
					pluslen = strlen(gfunstr);
					sprintf(tempgfunstr + pluslen, "%lld#", h_point);
					pluslen = strlen(tempgfunstr);
					*(gfunstr + pluslen) = '\0';
				}
				else
				{	
					pluslen = strlen(gfunstr);
					sprintf(tempgfunstr + pluslen, "%lld", h_point);
					pluslen = strlen(tempgfunstr);
					*(gfunstr + pluslen) = '\0';
				}


				tempsum+= (((r[n] * h_point) % tablesize) + tablesize) % tablesize ;	
								
			}

		
			hashresult = (((tempsum ) % tablesize) + tablesize ) % tablesize;
		}
		else if(metric == 2)
		{

		
			for (int n = 0; n < k; n++)
			{	
				h_point = h(hf[0][n], querypointnode, metric, n, dimension_of_coords);

				hashresult += h_point * pow(2, k-n-1);
			}
				

			
		}
		

		//NN
		//one possible NN from each hashtable
		struct points_hashed *tempp = NULL;
		//if ((table[y][hashresult] -> points_hashed_here))
		tempp = table[y][hashresult].firstpoint;
		//for each point in this bucket, I check and save the smallest distance
		int counter = 0;
		while (tempp!=NULL)
		{

			//printf("\n%s - %s\n", gfunstr, tempp-> gfunvector );
			int flagcheck = 0;
			if (metric == 1) 
			{
                //printf("\ngfunvector: %s\n", tempp-> gfunvector );
                if (!(strcmp(gfunstr, tempp -> gfunvector)))
                {
                    flagcheck = 1;

					tsum = 0.0;
					for (int o = 0; o < dimension_of_coords; o++)
					{
						tsum += pow ( fabs(( get_coord_multipoints(querypointnode, o)) - (get_coord_multipoints(tempp->point, o))), 2);
					}
					tsum = sqrt(tsum);
					//printf("\nTsum = %.4lf\n", tsum);
					
                }
			}
			else if (metric == 2)
			{
				tsum = 1 - cos_v(tempp -> point, querypointnode, dimension_of_coords );

				flagcheck = 1;
			}

            
			if (flagcheck)
			{
				//if((radius>0) && (tsum <= radius)) 
				//{
                    
					r_neighbours = 1;

					if (rlist == NULL) //first neighbour
					{

						struct simple_list* newnode = NULL;
						newnode = malloc(sizeof(struct simple_list));
						newnode -> next = NULL;
						newnode -> point = tempp -> point;
						newnode -> distance = tsum;
						rlist = newnode;

                        //neighbour_tot_num++;
					}
					else //if i already have at least 1 R-neighbour in list, insert this one if not already in list
					{	

						struct simple_list* rlistcheck_keep = NULL, *rlistcheck = NULL;
						int flagrlist = 0; //check if this point is already in the r-neighbours list
						rlistcheck_keep = rlist;
						rlistcheck = rlist;
						while (rlistcheck!= NULL)
						{
                            //printf("\n%s - to add: %s\n", get_name_multipoints( rlistcheck -> point), get_name_multipoints( tempp -> point));
							if (!strncmp( get_name_multipoints( tempp -> point), get_name_multipoints( rlistcheck -> point) , strlen(get_name_multipoints( rlistcheck -> point) )))
							{
								flagrlist = 1;

								break;
							}
							

							rlistcheck_keep = rlistcheck;
							rlistcheck = rlistcheck -> next;

						}

						if (!flagrlist) //if this point is not yet in the list, insert it
						{	

							struct simple_list* newnode = NULL;
							newnode = malloc(sizeof(struct simple_list));
							newnode -> next = NULL;
							newnode -> point = tempp -> point;
							newnode -> distance = tsum;
							rlistcheck_keep -> next = newnode;

                            //neighbour_tot_num++;

						}

						//r_neighbours = 1;
					}

				//}

			}

           

			
			counter++;
			tempp = tempp -> next;

		}

		//printf("Min dist: %.4lf, point id: %d\n", NNmindist, NNid );
		if (metric == 1)
			free(gfunstr);
	}
	

    //no_new_points = assign_points_lsh_algo(rlist, clusters_ptr, cluster_id, clusters_num, metric );
	//print_rlist(rlist);

    return rlist;

}

int LSH_algo(long long totpointsin, struct multipoints* querypointnode, int k, int L, struct multipoints* points, point_num_type tablesize, struct bucket *table[], struct hfun* hf[][k], int r[], int metric,  double radius, int cluster_id, struct cluster* clusters_ptr[], int clusters_num, int dimension_of_coords)
{
    point_num_type no_new_points = 1;
	

    long long hashresult = 0;
	long long tempsum = 0;
	long long h_point = 0;

	//for Range Search - if radius was > 0
	struct simple_list* rlist = NULL;

	double tsum = 0.0;
	int r_neighbours = 0;

	//printf("\nSearching for neighbours..\n");
    //printf("Centroid: %s " , get_name_multipoints( querypointnode) );
    for (int y = 0; y < L; y++)
	{
		hashresult = 0;

		tempsum = 0;
		//hlist = gfuns[y] -> hf_index; //the first h of the hash function(g) of this hashtable
		char *gfunstr = NULL;
		if (metric == 1)
		{
			
			int strsize = k*sizeof(long long) + (k-1)*sizeof("#") + 1;
			gfunstr = malloc(strsize);
			char *tempgfunstr = NULL;
			tempgfunstr = gfunstr;
			int pluslen = 0;

			for (int n = 0; n < k; n++)
			{	
				//printf("Point%d\nHindex: %d - n %d", querypointnode -> coords[d-1], hlist->h_index, n);
			
				h_point = h(hf[y][n], querypointnode, metric, n, dimension_of_coords);
				
				
				//h_point = h(hf[y][n], querypointnode, metric, n);


				if (n == 0)
				{	
					sprintf(tempgfunstr + pluslen, "%lld#", h_point);
					pluslen = strlen(tempgfunstr);
					*(gfunstr + pluslen) = '\0';
				}
				else if (n<k-1)
				{
					pluslen = strlen(gfunstr);
					sprintf(tempgfunstr + pluslen, "%lld#", h_point);
					pluslen = strlen(tempgfunstr);
					*(gfunstr + pluslen) = '\0';
				}
				else
				{	
					pluslen = strlen(gfunstr);
					sprintf(tempgfunstr + pluslen, "%lld", h_point);
					pluslen = strlen(tempgfunstr);
					*(gfunstr + pluslen) = '\0';
				}


				tempsum+= (((r[n] * h_point) % tablesize) + tablesize) % tablesize ;	
								
			}

		
			hashresult = (((tempsum ) % tablesize) + tablesize ) % tablesize;
		}
		else if(metric == 2)
		{

		
			for (int n = 0; n < k; n++)
			{	
				h_point = h(hf[0][n], querypointnode, metric, n, dimension_of_coords);

				hashresult += h_point * pow(2, k-n-1);
			}
				

			
		}
		

		//NN
		//one possible NN from each hashtable
		struct points_hashed *tempp = NULL;
		//if ((table[y][hashresult] -> points_hashed_here))
		tempp = table[y][hashresult].firstpoint;
		//for each point in this bucket, I check and save the smallest distance
		int counter = 0;
		while (tempp!=NULL)
		{

			//printf("\n%s - %s\n", gfunstr, tempp-> gfunvector );
			int flagcheck = 0;
			if (metric == 1) 
			{
                //printf("\ngfunvector: %s\n", tempp-> gfunvector );
                if (!(strcmp(gfunstr, tempp -> gfunvector)))
                {
                    flagcheck = 1;

					tsum = 0.0;
					for (int o = 0; o < dimension_of_coords; o++)
					{
						tsum += pow ( fabs(( get_coord_multipoints(querypointnode, o)) - (get_coord_multipoints(tempp->point, o))), 2);
					}
					tsum = sqrt(tsum);
					//printf("\nTsum = %.4lf\n", tsum);
					
                }
			}
			else if (metric == 2)
			{
				tsum = 1 - cos_v(tempp -> point, querypointnode , dimension_of_coords);

				flagcheck = 1;
			}

            
			if (flagcheck)
			{
				if((radius>0) && (tsum <= radius)) 
				{
                    
					r_neighbours = 1;

					if (rlist == NULL) //first neighbour
					{

						struct simple_list* newnode = NULL;
						newnode = malloc(sizeof(struct simple_list));
						newnode -> next = NULL;
						newnode -> point = tempp -> point;
						rlist = newnode;

                        //neighbour_tot_num++;
					}
					else //if i already have at least 1 R-neighbour in list, insert this one if not already in list
					{	

						struct simple_list* rlistcheck_keep = NULL, *rlistcheck = NULL;
						int flagrlist = 0; //check if this point is already in the r-neighbours list
						rlistcheck_keep = rlist;
						rlistcheck = rlist;
						while (rlistcheck!= NULL)
						{
                            //printf("\n%s - to add: %s\n", get_name_multipoints( rlistcheck -> point), get_name_multipoints( tempp -> point));
							if (!strcmp( get_name_multipoints( tempp -> point), get_name_multipoints( rlistcheck -> point) ))
							{
								flagrlist = 1;

								break;
							}
							

							rlistcheck_keep = rlistcheck;
							rlistcheck = rlistcheck -> next;

						}

						if (!flagrlist) //if this point is not yet in the list, insert it
						{	

							struct simple_list* newnode = NULL;
							newnode = malloc(sizeof(struct simple_list));
							newnode -> next = NULL;
							newnode -> point = tempp -> point;
							rlistcheck_keep -> next = newnode;

                            //neighbour_tot_num++;

						}

						//r_neighbours = 1;
					}

				}

			}

           

			
			counter++;
			tempp = tempp -> next;

		}

		//printf("Min dist: %.4lf, point id: %d\n", NNmindist, NNid );
		if (metric == 1)
			free(gfunstr);
	}
	

    no_new_points = assign_points_lsh_algo(rlist, clusters_ptr, cluster_id, clusters_num, metric , dimension_of_coords);


    return no_new_points;

}


//printing functions -------------------------------------------------------------------------------------------------------------------------------------------------

void printhfuns(int k, int L, struct hfun *list[][k], int dimension_of_coords)
{
	for (int j = 0; j < L; j++)
	{
		for (int i = 0; i < k; i++)
		{
			printf("\nH function %d:\nVector v: ", i);
			for (int l = 0; l < dimension_of_coords; l++)
			{
				printf("%.4f ", list[j][i]->v->vcoords[0][l]);
			}

			printf("\nt = %.4f", list[j][i]->t);
		}
	}
}


void print_hashtable(point_num_type tablesize, int L, struct bucket *table[])
{

	struct points_hashed *tempoint = NULL;
	for (int p = 0; p < L; p++)
    {
        for (point_num_type u = 0; u < tablesize; u++)
        {
            printf("\nTable%d: Bucket%lld: Points: ", p, u);
        	tempoint = table[p][u].firstpoint;
        	while (tempoint!=NULL)
        	{

        		printf("%s ", get_name_multipoints( tempoint -> point ));
        		tempoint = tempoint -> next;
        	}


        }
    }


}

//delete functions

int delete_hfun(int k, int L, struct hfun* list[][k], int metric)
{

	if (metric == 1)
	{
		for (int j = 0; j < L; j++)
		{	for (int o = 0; o < k; o++)
			{
				free(list[j][o]->v->vcoords[0]);
				free(list[j][o]->v);             //frees the node and backtraces
				list[j][o]->v = NULL;
				free(list[j][o]);  
			}
		}
	}
	else if(metric ==2)
	{

		for (int o = 0; o < k; o++)
		{
			free(list[0][o]->v->vcoords[0]);
			free(list[0][o]->v);             //frees the node and backtraces
			list[0][o]->v = NULL;
			free(list[0][o]);  
		}

	}
		      
    return 4;  
 
   
}

void delete_hfun_cube(struct hfun* list[], int k)
{

    for (int o = 0; o < k; o++)
    {
		free(list[o]->v->vcoords[0]);
        free(list[o]->v);             
        list[o]->v = NULL;
        free(list[o]);  
    }
	
 
}

int delete_hflist(struct hf_list *list)
{


    if((list->next) !=NULL)  
    {   
        delete_hflist(list->next);  
    }  
          

    list-> next = NULL;    
    free(list);             //free ton komvo kai epistrefei 


	return 8;
}



int delete_pointshashed(struct points_hashed *list)
{
	struct points_hashed *temp1 = NULL, *temp2 = NULL;
	temp1 = list;
	while (temp1 != NULL)
	{
		temp2 = temp1 -> next;
		free(temp1->gfunvector);
		free(temp1);
		temp1 = temp2;

	}

	free(temp1);
        
	return 9;

}


int delete_hashtablebucket(struct bucket bucket)
{

	delete_pointshashed(bucket.firstpoint);

    bucket.firstpoint = NULL;

    bucket.lastpoint = NULL;

	return 7;

}

int delete_hashtablebucket_cube(struct bucket* bucket[], long long bucketnum)
{

    delete_pointshashed(bucket[0][bucketnum].firstpoint);

    bucket[0][bucketnum].firstpoint = NULL;

    bucket[0][bucketnum].lastpoint = NULL;

    return 72;

}

void delete_all_lsh(int k, int L, struct hfun* hfarray[][k], point_num_type tablesize, struct bucket* hashtablesarray[], int metric)
{

    //delete_multipointlist(MpointsList);
    
    delete_hfun(k, L, hfarray, metric);

    //deletes hashtable
    for (int p = 0; p < L; p++)
    {

        for (point_num_type u = 0; u < tablesize; u++)
        {
            delete_hashtablebucket(hashtablesarray[p][u]);  
        }

        free(hashtablesarray[p] );
        hashtablesarray[p] = NULL;
    }

}


