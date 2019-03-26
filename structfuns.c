//
//  structfuns.c
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#include "structs.h"

//structs

//struct multipoints;
struct multipoints
{
    //for the recommendation
    int real;

    //for the assignment
    int centroid; //and recommendation
    int locked;
    //coordinates of point
    coordtype coords[d];
    coordtype* coords_array[1];
    //point id number
    long long idnum; //it was used in another version of the program when I was creating the id inside the program
    //point name
    char *name;
    
    struct multipoints *next;

    //int name_or_id; // a flag to check if i have a number as id or a string.... !!!!!!!!!!!!!!!
};

//struct points in cluster
struct points_in_cluster
{
	struct multipoints *point;

	struct points_in_cluster *next;
};

//struct cluster
struct cluster
{
	point_num_type totpoints_in_cluster;

    //coordinates of point
    int cluster_id; //k clusters

    //centroid point
    struct multipoints *cluster_centroid; //it can a pointer to a "Mean" struct multipoints node or a pointer to a point from MpointsList(dataset)

    //coordinates of centroid - if not in dataset
    //coordtype coords[d];

    //list of points in cluster
    struct points_in_cluster* list_of_points_first;

    struct points_in_cluster* list_of_points_last;


};



//get size of structures
size_t get_size_multipoints()
{   return sizeof(struct multipoints); }

size_t get_size_cluster()
{   return sizeof(struct cluster); }

size_t get_size_points_in_cluster()
{   return sizeof(struct points_in_cluster); }



// init
struct multipoints* newnode_multipoints(int dimension_of_coords)
{   
    struct multipoints* newnode = NULL;
    //printf("\n%ld(dimension:%d)\n", get_size_multipoints(), dimension_of_coords);
    newnode = malloc(get_size_multipoints());
    //printf("\nnewnode:%d\n", __LINE__);
    newnode -> real = 0;
    newnode -> centroid = -9;
    newnode -> locked = 0;
    memset((void*)(newnode->coords), 0, dimension_of_coords*sizeof(coordtype));

    newnode -> coords_array[0] = malloc(dimension_of_coords*sizeof(coordtype));
    memset((void*)(newnode->coords_array[0]), 0, dimension_of_coords*sizeof(coordtype));

    newnode -> idnum = 0;
    newnode -> name = NULL;
    newnode -> next = NULL;
   
    return newnode; 
}

struct points_in_cluster* newnode_points_in_cluster(void)
{   
    struct points_in_cluster* newnode = NULL;
    newnode = malloc(get_size_points_in_cluster());

    newnode -> point = NULL;
    newnode -> next = NULL;


    return newnode; 
}

struct cluster* newnode_cluster(void)
{   
    struct cluster* newnode = NULL;
    newnode = malloc(get_size_cluster());

	newnode -> totpoints_in_cluster = 0;
	newnode -> cluster_id = -9;
	newnode -> cluster_centroid = NULL;
    //memset((void*)(newnode->coords), 0, d*sizeof(coordtype));
    newnode -> list_of_points_first = NULL;
    newnode -> list_of_points_last = NULL;
  
    return newnode; 
}



//struct manipulations funs

//multipoints
//basic
struct multipoints* multiPointList_init(int dimension_of_coords)  //initial node of our points list
{
    struct multipoints *list = NULL;
    list = (struct multipoints *) malloc(sizeof(struct multipoints));
    if(list == NULL)
    {
        printf("Memory allocation failed");
        //return NULL;
    }
    else
    {
        (list -> coords_array[0]) = malloc(dimension_of_coords*sizeof(coordtype));
        for(int k = 0; k < dimension_of_coords; k++)
        {
            list -> coords_array[0][k] = 0;
        }
        list -> real = 0;
        list -> centroid = -9;
        list -> locked = 0;
        list -> idnum = 0;
        list -> name = NULL;
        list -> next = NULL;
        //memset((void*)list -> coords, 0, dimension_of_coords*sizeof(int));
        //memset((void*)list -> coords_array[0], 0, dimension_of_coords*sizeof(coordtype));

    }
    
    return list;
    
}

int multiPointList_insertxcoord(struct multipoints* multipointnode, coordtype xcoord, point_num_type xnum)
{
    
    //multipointnode->coords[xnum] = xcoord;
    multipointnode -> coords_array[0][xnum] = xcoord;
    //printf("\nCoord %lf", xcoord);

    return 7;
}

struct multipoints* multiPointList_insertpoint(struct multipoints* multipointnode, char* name, int dimension_of_coords)
{
    //printf("\nInsertPoint - %s", name);
    if ((multipointnode->name)==NULL)//(multipointnode -> idnum == -9)
    {

        multipointnode -> name = name;
        multipointnode -> next = NULL;
        return multipointnode;
    }
    else
    {
        struct multipoints *newnode = malloc(sizeof(struct multipoints));

        if(newnode == NULL)
        {
            printf("Memory allocation failed");
            return NULL;
        }

        newnode -> real = 0;

        newnode -> centroid = -9;
        newnode -> locked = 0;

        memset((void*)newnode -> coords, 0, dimension_of_coords*sizeof(coordtype));
        newnode -> coords_array[0] = malloc(dimension_of_coords*sizeof(coordtype));
        memset((void*)newnode -> coords_array[0], 0, dimension_of_coords*sizeof(coordtype));
        
        newnode -> next = NULL;
        newnode -> name = name;
        //printf("\nPoint: %s inserted!\n", newnode->name);
        multipointnode -> next = newnode;
        return newnode;
    }
    
}

//get
coordtype get_coord_multipoints(struct multipoints* point, int coordnum)
{   //return (point->coords[coordnum]); 
    // if(point->coords_array[0][coordnum] == INFINITY)
    // {
    //     printf("\ncoord of point: %s : %lf\n", point->name,point->coords_array[0][coordnum] );
    // }
    return (point->coords_array[0][coordnum]);
}

coordtype* get_coord_ptr_multipoints(struct multipoints* point)
{   //return (point -> coords); 
    return (point -> coords_array[0]);
}

struct multipoints* get_next_multipoints( struct multipoints* point)
{   return (point -> next); }

char* get_name_multipoints(struct multipoints* point)
{   return (point -> name); }

int get_centroid_multipoints(struct multipoints* point)
{   return (point -> centroid); }

int get_locked_multipoints(struct multipoints* point)
{   return (point -> locked); }

int get_real_multipoints(struct multipoints* point)
{   return (point -> real) ; }

//save
void save_centroid_multipoints(struct multipoints* point, int centroid)
{   (point -> centroid = centroid); }

void save_locked_multipoints(struct multipoints* point, int locked)
{   (point -> locked = locked); }

void save_name_multipoints(struct multipoints* point, char* name)
{   point -> name = name; }

void save_idnum_multipoints(struct multipoints* point, int idnum)
{   point -> idnum = idnum; }

void save_coord_multipoints(struct multipoints* point, coordtype coord, int coordnum)
{   //point -> coords[coordnum] = coord; 
    point -> coords_array[0][coordnum] = coord;
}

void save_next_multipoints(struct multipoints* point, struct multipoints* next)
{   point -> next = next; }

void save_real_multipoints(struct multipoints* point, int real)
{   point -> real = real; }


//delete
void multiPointList_delete_next_node(struct multipoints* node)
{
    delete_multipoint(node->next);
    node -> next = NULL;

}


//interpoints funs
coordtype euclidean_dist(struct multipoints* point1, struct multipoints* point2, int dimension_of_coords)  //formula of euclidean distance
{	
	coordtype dist = 0.0, tempdist = 0.0, x = 0.0, x1 = 0.0, x2 = 0.0;
    
	for (int u = 0; u < dimension_of_coords; u++)
	{
        x1 = get_coord_multipoints(point2, u);
        x2 = get_coord_multipoints(point1, u);
        x = fabs( (double) x1 -  (double) x2);
        tempdist += pow(x, 2);

        // if(tempdist <0)
        // {
        //     printf("\nNEGval-structfuns.c:%d\n", __LINE__);

        // }
        // else if(tempdist == INFINITY)
        // {
        //     printf("\nINF-structfuns.c:%d\n", __LINE__);

        // }
        //tempdist += pow ( fabs( (double) ( (double) get_coord_multipoints(point2, u) ) - ( (double) get_coord_multipoints(point1, u) )), 2);
        //printf("\nTempdist: %lf (%lf - %lf) fabs %lf", tempdist, get_coord_multipoints(point2, u), get_coord_multipoints(point1, u) , fabs( (double) ( (double) get_coord_multipoints(point2, u) ) - ( (double) get_coord_multipoints(point1, u) )));
        //  printf("\npoint1(%s)-point2(%s): %ld = |%lf - %lf| = %lf\n", get_name_multipoints(point1), get_name_multipoints(point1), u, x, x1, x2, tempdist);
    }
    

    dist = sqrt(tempdist);

	return dist;
}

coordtype euclidean_sim(struct multipoints* point1, struct multipoints* point2, int dimension_of_coords)  //formula of euclidean distance
{
    coordtype sim = 0.0, dist = 0.0;
    dist = euclidean_dist(point1, point2, dimension_of_coords);

    // if( (1+dist) == 0 )
    // {
    //     printf("\n ZERO-structfuns.c:%d \n", __LINE__);
    // }
    // if( dist == INFINITY )
    // {
    //     printf("\n INF-structfuns.c:%d \n", __LINE__);

    // }

    sim = (double) (1 /(double) ( 1 + dist ) );
    //printf("\n sim = %lf \n", sim);

    return sim;
    
}

coordtype cos_v(struct multipoints* x, struct multipoints* y, int dimension_of_coords)
{

	coordtype my_cos = 0.0, normx = 0.0, normy = 0.0, x_y = 0.0;

	for (int i = 0; i < dimension_of_coords; i++)
	{

		normx += pow( get_coord_multipoints(x , i), 2);
		normy += pow( get_coord_multipoints(y , i), 2);
		x_y += (get_coord_multipoints(x , i)) * (get_coord_multipoints(y , i));

	}

	normx = sqrt(normx);
	normy = sqrt(normy);

    double tempp = 0.0;
    tempp = normx * normy;
    if(!tempp)
    {
	    my_cos = 0;
    }
    else
    {
        my_cos = (coordtype)x_y/(coordtype)(normx * normy);
    }


	return my_cos;


}

coordtype cosine_dist(struct multipoints* point1, struct multipoints* point2, int dimension_of_coords)  //formula of cosine distance
{	
	coordtype dist = 0.0;

	dist = 1 - cos_v(point1, point2, dimension_of_coords);

	return dist;
}

coordtype get_dist_multipoints(struct multipoints* point1, struct multipoints* point2, int metric, int dimension_of_coords) //get distance between 2 points
{
	coordtype dist = 0.0;

	if(metric == 1)
	{
		dist = euclidean_dist(point1, point2, dimension_of_coords);
	}
	else if(metric == 2)
	{
		dist = cosine_dist(point1, point2, dimension_of_coords);
	}

	return dist;
}

int check_if_same(struct multipoints* point1, struct multipoints* point2, int dimension_of_coords )  //check two points if they are the same
{
    int coords_same = 1;
    if(!(strcmp(get_name_multipoints(point1), get_name_multipoints(point2))))
    {
        for(int k = 0; k < dimension_of_coords; k++)
        {
            if(( get_coord_multipoints(point1, k) )!= ( get_coord_multipoints(point2, k) ))
            {
                coords_same = 0;
                return 0;
            }
        }

        if(coords_same == 1)
        {   
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }



}


//cluster
//init
struct cluster* clusters_init(int clusters_num, struct cluster* clusters_ptr[])
{

	for(int u = 0; u < clusters_num; u++)
	{ 
		clusters_ptr[0][u].totpoints_in_cluster = 0;
		clusters_ptr[0][u].cluster_id = -9;
		clusters_ptr[0][u].cluster_centroid = NULL;
	    //memset((void*)(clusters_ptr[0][u].coords), 0, d*sizeof(coordtype));
	    clusters_ptr[0][u].list_of_points_first = NULL;
	    clusters_ptr[0][u].list_of_points_last = NULL;
	}

	return NULL;
}

void clear_clusters(struct cluster* clusters_ptr[], int clusters_num)  //clear clusters between the 12 combinations of algorithms
{

    for (int i = 0; i < clusters_num; i++)
    {
        if((clusters_ptr[0][i].cluster_centroid)!=NULL)
        {
            if(!(strncmp((clusters_ptr[0][i].cluster_centroid ) -> name, "Mean", strlen("Mean"))))
            {
                delete_multipoint(clusters_ptr[0][i].cluster_centroid);
            }
            else
            {
                clusters_ptr[0][i].cluster_centroid = NULL;
            }
        }

        if(clusters_ptr[0][i].totpoints_in_cluster)
        {
            clusters_ptr[0][i].totpoints_in_cluster = 0;
            delete_points_in_cluster_list(clusters_ptr[0][i].list_of_points_first);
            clusters_ptr[0][i].list_of_points_first = NULL;
            clusters_ptr[0][i].list_of_points_last = NULL;
        }


    }

}

//get
point_num_type get_points_in_cluster(int cluster_id, struct cluster* clusters_ptr[])
{   return ( clusters_ptr[0][cluster_id].totpoints_in_cluster ); }

int get_cluster_id(int cluster_id, struct cluster* clusters_ptr[])
{	return (clusters_ptr[0][cluster_id].cluster_id); }

struct multipoints* get_cluster_centroid(int cluster_id, struct cluster* clusters_ptr[])
{	return (clusters_ptr[0][cluster_id].cluster_centroid); }

struct points_in_cluster* get_lastpoint_in_cluster(int cluster_id, struct cluster* clusters_ptr[])
{	return ( clusters_ptr[0][cluster_id].list_of_points_last  ); }

struct points_in_cluster* get_firstpoint_in_cluster(int cluster_id, struct cluster* clusters_ptr[])
{	return ( clusters_ptr[0][cluster_id].list_of_points_first  ); }


//save
void save_totpoints_in_cluster(int cluster_id, struct cluster* clusters_ptr[], point_num_type points_in_cluster)
{	( clusters_ptr[0][cluster_id].totpoints_in_cluster ) = points_in_cluster; }

void save_cluster_id(int c_id, struct cluster* clusters_ptr[])
{	( clusters_ptr[0][c_id].cluster_id ) = c_id; }

void save_cluster_centroid(int cluster_id, struct cluster* clusters_ptr[], struct multipoints* centroid)
{	( clusters_ptr[0][cluster_id].cluster_centroid ) = centroid; }

void save_list_of_points_first(int cluster_id, struct cluster* clusters_ptr[], struct points_in_cluster* point)
{ 	( clusters_ptr[0][cluster_id].list_of_points_first ) = point; }

void save_list_of_points_last(int cluster_id, struct cluster* clusters_ptr[], struct points_in_cluster* point)
{	( clusters_ptr[0][cluster_id].list_of_points_last ) = point; }





//points_in_cluster
//get
struct multipoints* get_point_points_in_cluster(struct points_in_cluster* point_in_cluster)
{   return (point_in_cluster -> point); }

struct points_in_cluster* get_next_points_in_cluster(struct points_in_cluster* point_in_cluster)
{   return (point_in_cluster -> next); }

//save
void save_point_points_in_cluster(struct points_in_cluster* point_in_cluster, struct multipoints* point_to_save)
{	point_in_cluster -> point = point_to_save; }

void save_next_points_in_cluster(struct points_in_cluster* point, struct points_in_cluster* next)
{	point -> next = next; }


//add point to a cluster when points are being assigned
struct points_in_cluster* add_point_to_cluster(struct multipoints* point, int cluster_id, struct cluster* clusters_ptr[])
{
    int found = 0;

    if(!found)
    {
        point_num_type tpoints = 0;
    	if ((tpoints = clusters_ptr[0][cluster_id].totpoints_in_cluster)==0) //get_points_in_cluster(cluster_id, clusters_ptr)) == 0)
    	{
            struct points_in_cluster* newnode = NULL;
            newnode = newnode_points_in_cluster();

            newnode -> next = NULL;
            newnode -> point = point;
            clusters_ptr[0][cluster_id].list_of_points_first = newnode;
            clusters_ptr[0][cluster_id].list_of_points_last = clusters_ptr[0][cluster_id].list_of_points_first;
            clusters_ptr[0][cluster_id].totpoints_in_cluster = 1;


    	}
    	else
    	{
            struct points_in_cluster* newnode= NULL;
            newnode = newnode_points_in_cluster();

            newnode -> next = NULL;
            newnode -> point = point;
            
            clusters_ptr[0][cluster_id].list_of_points_last -> next = newnode;
            clusters_ptr[0][cluster_id].list_of_points_last = newnode;

            (clusters_ptr[0][cluster_id].totpoints_in_cluster) += 1;

            //printf("\nPoint: %s (centroid: %d - lock: %d) added to Cluster-%d\n", get_name_multipoints(get_point_points_in_cluster(clusters_ptr[0][cluster_id].list_of_points_last)) ,  get_centroid_multipoints(point), get_locked_multipoints(point) , cluster_id );

    	}
    }

	return NULL;
}


//delete point from a certain cluster before assigning it to another one
void delete_point_from_cluster(struct multipoints* point, int cluster_id, struct cluster* clusters_ptr[])
{
    struct points_in_cluster* templist = NULL, *templist_prev = NULL;
    
    templist = clusters_ptr[0][cluster_id].list_of_points_first;
    templist_prev = templist;
    point_num_type point_counter = 0;
    int found = 0;
    while(templist!=NULL)
    {
        //printf("\nlist: %s delete-> %s \n", get_name_multipoints(get_point_points_in_cluster(templist)),get_name_multipoints(point) );
        if (!(strcmp(get_name_multipoints(get_point_points_in_cluster(templist)), get_name_multipoints(point))))
        {

            if(!point_counter)  //first point
            {
                save_list_of_points_first(cluster_id, clusters_ptr, get_next_points_in_cluster(templist));
                templist -> point = NULL;
                templist -> next = NULL;
                free(templist);
            }
            else if(point_counter == (get_points_in_cluster(cluster_id, clusters_ptr) - 1)) //last point
            {
                save_next_points_in_cluster(templist_prev, NULL);
                clusters_ptr[0][cluster_id].list_of_points_last = templist_prev;
                templist -> point = NULL;
                templist -> next = NULL;
                free(templist); 
            }
            else    //point_between
            {
                save_next_points_in_cluster(templist_prev, get_next_points_in_cluster(templist));

                templist -> point = NULL;
                templist -> next = NULL;
                free(templist);
            }



            found = 1;
            break;
        }


        templist_prev = templist;
        templist = get_next_points_in_cluster(templist);
        point_counter++;

    }

    if (!found)
    {
        printf("\nERROR: Point: %s not in Cluster-%d\n", get_name_multipoints(point), cluster_id );
    }
    else
    {

        (clusters_ptr[0][cluster_id].totpoints_in_cluster) -= 1;

        if ((clusters_ptr[0][cluster_id].totpoints_in_cluster)==0)
        {
            clusters_ptr[0][cluster_id].list_of_points_last = NULL;
        }
        else if((clusters_ptr[0][cluster_id].totpoints_in_cluster) == 1)
        {
            clusters_ptr[0][cluster_id].list_of_points_last = clusters_ptr[0][cluster_id].list_of_points_first;
        }   

        //printf("\nPoint: %s(centroid: %d - lock: %d) deleted from Cluster-%d\n", get_name_multipoints(point), get_centroid_multipoints(point), get_locked_multipoints(point) , cluster_id );

    }


    

}

//printing funs
void print_point(struct multipoints* point, int dimension_of_coords)
{
    printf("\nPoint: %s\nCoordinates: ", point->name );
    for (int i = 0; i < dimension_of_coords; i++)
    {
        printf("%lf ", point->coords_array[0][i]);
    }
}


void printList(struct multipoints* list, int dimension_of_coords)
{
    struct multipoints* temp = NULL;
    temp = list;
    printf("\nPrinting Points..");
    //printf("\ntemp: %d..", temp->idnum);

    while (temp!=NULL)
    {
        // printf("\nId: %d\nCoordinates: ", temp->idnum);
        // for (int k = 0; k<d; k++)
        //     printf("%d ", temp->coords[k]);
        print_point(temp, dimension_of_coords);
        
        temp = temp->next;
    }

    printf("\n");
    
}

void printClusters(struct cluster* clusters_ptr[], int clusters_num)
{
    struct points_in_cluster* temp = NULL;
    //temp = list;
    printf("\nPrinting Clusters..");
    //printf("\ntemp: %d..", temp->idnum);

    print_msg_centre(" Clusters ");
    print_line();
    printf("\n\n");
    for(int y = 0; y < clusters_num; y++)
    {  
        char* tempstr = NULL, *tempsizestr = NULL;
        int tsize = strlen(" Cluster  ") + sizeof(int) + 1;
        int tsize2 = strlen(" (  ) ") + sizeof(point_num_type);
        tempstr = malloc(sizeof(char) * tsize);
        tempsizestr = malloc(sizeof(char) * tsize2);
        sprintf(tempstr, " Cluster %d " , y);
        sprintf(tempsizestr, " ( %lld ) ", get_points_in_cluster(y, clusters_ptr));
        print_msg_centre_line(tempstr);
        print_msg_centre(tempsizestr);
        printf("\n\n");
        //printf("\nCluster %d:", y);
        temp = get_firstpoint_in_cluster(y, clusters_ptr);
        while (temp!=NULL)
        {
            printf(" %s ", get_name_multipoints(get_point_points_in_cluster(temp)));
         
            temp = get_next_points_in_cluster(temp);
        }

        printf("\n");
    }

    printf("\n");
    
}

void print_cluster_size(struct cluster* clusters_ptr[], int clusters_num)
{

    for(int y = 0; y < clusters_num; y++)
    {  
        char* tempstr = NULL, *tempsizestr = NULL;
        int tsize = strlen(" Cluster  ") + sizeof(int) + 1;
        int tsize2 = strlen(" (  ) ") + sizeof(point_num_type);
        tempstr = malloc(sizeof(char) * tsize);
        tempsizestr = malloc(sizeof(char) * tsize2);
        sprintf(tempstr, " Cluster %d " , y);
        sprintf(tempsizestr, " ( %lld ) ", get_points_in_cluster(y, clusters_ptr));
        print_msg_centre_line(tempstr);
        print_msg_centre(tempsizestr);
        printf("\nCentroid: %s\n", get_name_multipoints( get_cluster_centroid(y, clusters_ptr)) );
        printf("\n\n");
        //printf("\nCluster %d:", y);

    }

    printf("\n");
}


void print_centroids(struct cluster* clusters_ptr[], int clusters_num)
{

    for(int y = 0; y < clusters_num; y++)
    {  
        printf("\nCentroid[%d]: %s\n", y, get_name_multipoints( get_cluster_centroid(y, clusters_ptr)) );
        printf("\n");
    }
    printf("\n");

}

void print_rlist(struct simple_list* rlist)
{
    struct simple_list* templist = NULL;
    templist = rlist;
    long long counter = 0;
    printf("\nRlist:");
    while (templist!= NULL)
    {

        printf("\nindex:|%s|(%.15lf)| ", get_name_multipoints( get_point_simple_list(templist)), get_distance_simple_list(templist) );    

        templist = get_next_simple_list(templist);
        counter++;

    }
}



//delete functions
void delete_multipoint(struct multipoints* point)
{
    free(point -> name );
    point -> name = NULL;
    point -> next = NULL;
    free(point -> coords_array[0]);
    free(point);
    point = NULL;

}

void delete_multipointlist(struct multipoints* list)
{
    struct multipoints *temp1 = NULL, *temp2 = NULL;
    temp1 = list;
    while (temp1 != NULL)
    {
        temp2 = temp1 -> next;

        delete_multipoint(temp1);

        temp1 = temp2;

    }

}


void delete_points_in_cluster_list(struct points_in_cluster* list)
{

    struct points_in_cluster* temp1 = NULL, *temp2 = NULL;
    temp1 = list;
    //temp2 = temp1;
    while(temp1!=NULL)
    {
        temp2 = temp1 -> next;

        temp1 -> point = NULL;
        temp1 -> next = NULL;

        free(temp1);
        temp1 = temp2;
    }

}


void delete_clusters_2_2(int clusters_num, struct cluster* clusters_ptr[])
{

    for(int i = 0; i < clusters_num; i++)
    {


        delete_points_in_cluster_list(clusters_ptr[0][i].list_of_points_first);

        clusters_ptr[0][i].list_of_points_first = NULL;
        clusters_ptr[0][i].list_of_points_last = NULL;


        clusters_ptr[0][i].cluster_centroid = NULL;



    }

    free(clusters_ptr[0]);
    clusters_ptr = NULL;

}

void delete_clusters(int clusters_num, struct cluster* clusters_ptr[])
{

    for(int i = 0; i < clusters_num; i++)
    {


        delete_points_in_cluster_list(clusters_ptr[0][i].list_of_points_first);

        clusters_ptr[0][i].list_of_points_first = NULL;
        clusters_ptr[0][i].list_of_points_last = NULL;

        if (!(strncmp(get_name_multipoints(get_cluster_centroid(i, clusters_ptr)), "Mean" , strlen("Mean"))))
        {
            delete_multipoint(get_cluster_centroid(i, clusters_ptr));
            
        }

        clusters_ptr[0][i].cluster_centroid = NULL;



    }

    free(clusters_ptr[0]);
    clusters_ptr = NULL;

}

void delete_rlist(struct simple_list* rlist)
{
    struct simple_list *temp1 = NULL, *temp2 = NULL;
    temp1 = rlist;
    while (temp1 != NULL)
    {
        temp2 = get_next_simple_list(temp1);

        free(temp1);

        temp1 = temp2;

    }
    rlist = NULL;
}


void delete_all(struct multipoints* Mpointslist, struct cluster* clusters_ptr[], int clusters_num)
{

    delete_clusters(clusters_num, clusters_ptr);

    delete_multipointlist(Mpointslist);

    
}



