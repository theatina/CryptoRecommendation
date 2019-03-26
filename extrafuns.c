//
//  extrafuns.c
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#include "extrafuns.h"

//extra helpful

double my_log( int base, long long x )
{ 
    return log(x) / log(base); 
} 

int str_isdigit(const char *str) //if it's digit it returns 1
{
    const char* temp = str ;
    char xarakt = *temp;
    int isdig = 1;
    //printf("\nisdigit: word - %s\n", str);
    while((xarakt != '\0') && isdig)
    {
        if (isdigit(xarakt)==0)
            isdig = 0;
        temp++;
        xarakt = *temp;
    }
    
    return isdig;
    
}

//printing funs
void print_line(void)
{
    struct winsize win2;
    ioctl(0, TIOCGWINSZ, &win2);
    int width = (win2.ws_col);
    printf("\n");
    for (int f = 0; f < width; f++)
    {	printf("-"); }

}

void print_msg_centre_line(char* msg)
{
    struct winsize win2;
    ioctl(0, TIOCGWINSZ, &win2);
    int width = (win2.ws_col);
    int tempmsgsize = strlen(">  <") + strlen(msg)  ;
    int recwidth = (width - tempmsgsize)/2;
    
    printf("\n");

    for (int f = 0; f < recwidth; f++)
    {	printf("-"); }
    
    printf("> %s <", msg);
    
    for (int f = 0; f < recwidth; f++)
	{	printf("-"); }
    //printf("\n");
}

void print_msg_centre(char* msg)
{
    struct winsize win2;
    ioctl(0, TIOCGWINSZ, &win2);
    int width = (win2.ws_col);
    int tempmsgsize = strlen(msg)  ;
    int recwidth = (width - tempmsgsize)/2;
    printf("\n");

    for (int f = 0; f < recwidth; f++)
    {	printf(" "); }

    printf("%s", msg);

    for (int f = 0; f < recwidth; f++)
    {	printf(" "); }

}

void print_welcome_msg(void)
{
	print_line();
	printf("\n\n");
	print_msg_centre("Welcome to");
	printf("\n");
	print_msg_centre("the");
	printf("\n");
	print_msg_centre("CLUSTERing PROGRAM!");
	printf("\n\n");

}

void print_welcome_msg_recom(void)
{
	print_line();
	printf("\n\n");
	print_msg_centre("Welcome to");
	printf("\n");
	print_msg_centre("the");
	printf("\n");
	print_msg_centre("RECOMMENDATION PROGRAM!");
	printf("\n\n");

}


//more funs

int read_config_file(int* clusters_num, int* hfuns_num, int* hashtables_num, char* filename)
{
	FILE* fp = NULL ;
            
    if ((fp = fopen(filename, "r")) == NULL)
    {
        perror("\nfopen input-file\n");
        return -8;
    }

    size_t len = 0;
    //ssize_t linelen = 0;
    char delimit[]=" \t\n";
    
    char *token = NULL;

    while(!feof(fp))
    {
    	char *temp = NULL, *ttemp = NULL;
        //char *ttemp = temp;
        getline(&temp, &len, fp);  //error with uninitialised value!!! --- CHECK !!!!!!
        ttemp=temp;

        if(!(strncmp(temp, "number_of_clusters" , strlen("number_of_clusters"))))
        {
        	token = strtok_r(temp, delimit, &temp);
			token = strtok_r(temp, delimit, &temp);
			(*clusters_num) = atoi(token);
			// printf("\nNumber of clusters: %d\n",  (*clusters_num));
        }
        else if(!(strncmp(temp, "number_of_hash_functions" , strlen("number_of_hash_functions"))))
       	{	
       		token = strtok_r(temp, delimit, &temp);
			token = strtok_r(temp, delimit, &temp);
			(*hfuns_num) = atoi(token);
			// printf("\nNumber of h functions: %d\n",  (*hfuns_num));

       	}
       	else if(!(strncmp(temp, "number_of_hash_tables" , strlen("number_of_hash_tables"))))
       	{
       		token = strtok_r(temp, delimit, &temp);
			token = strtok_r(temp, delimit, &temp);
			(*hashtables_num) = atoi(token);
			// printf("\nNumber of hashTables: %d\n",  (*hashtables_num));
       	}

        free(ttemp);
    }

    fclose(fp);


	return 99;
}



//check input
//./cluster –i <input file> –c <configuration file> -o <output file> -d <metric>
int init_params(int argc, char* argv[], int* inputflag, char* inputfile, int* configflag, char* configfile, int* outputflag, char*outputfile, int* metric, int* clusters_num, int* hfuns_num, int* hashtables_num, int* complete_print)
{

    if ((argc > 10) || (argc < 2)) // if not inserted as arguments, the programm will ask
    {
        fprintf(stderr,"\nUsage: %s -i <input_file> -c <configuration_file> -o <output_file> -d <metric> -complete(for full presentation of results)\n\n", argv[0]);
        return -99;
    }

    // int inputflag = 0;
    // int queryflag = 0;
    // int outputflag = 0;


    for (int i=1; i < argc; i++)
    {
        if (!(strcmp(argv[i], "-i")))
        {
            (*inputflag) = 1;

            strcpy(inputfile, argv[i+1]);

            FILE *ftmp;
            if ((ftmp = fopen(inputfile, "r")) == NULL)
            {   //perror("\nfopen source-file"); 
                printf("\n"); 

                if (errno == ENOENT)
                {
                	(*inputflag) = 0;
                    printf("\nFile \"%s\" does not exist!\n", inputfile);
                    //return -1;

                }
                else if (errno == EACCES)
                {
                	(*inputflag) = 0;
                    printf("No Access rights for file \"%s\"\n", inputfile );
                    //return -2;

                }
                else
                {
                    (*inputflag) = 0;
                    printf("\nWeird error with file \"%s\"\n", inputfile );     
                }

            }

            if(*inputflag)
                fclose(ftmp);

            
        }
        else if (!(strcmp(argv[i], "-c")))
        {
            (*configflag) = 1;

            strcpy(configfile, argv[i+1]);

            FILE *fqtmp;
            if ((fqtmp = fopen(configfile, "r")) == NULL)
            {   //perror("\nfopen source-file"); 
                printf("\n"); 

                if (errno == ENOENT)
                {
                	(*configflag) = 0;
                    printf("\nFile \"%s\" does not exist!\n\n", configfile);
                    //return -1;

                }
                else if (errno == EACCES)
                {
                	(*configflag) = 0;
                    printf("No Access rights for file \"%s\"\n\n", configfile );
                    //return -2;

                }
                else
                {
                    (*configflag) = 0;
                    printf("\nWeird error with file \"%s\"\n\n", configfile );     
                }

            }

            if(*configflag)
                fclose(fqtmp);
    
        }
        else if (!(strcmp(argv[i], "-o")))
        {
            (*outputflag) = 1;

            strcpy(outputfile, argv[i+1]);

            FILE *foutmp;
            if ((foutmp = fopen(outputfile, "r")) == NULL)
            {   //perror("\nfopen source-file"); 
                printf("\n"); 

                if (errno == ENOENT)
                {
                	(*outputflag) = -9;
                    printf("\nFile \"%s\" does not exist!\n", outputfile);
                    printf("It will be created\n\n");


                    //return -1;

                }
                else if (errno == EACCES)
                {
                	(*outputflag) = 0;
                    printf("\nNo Access rights for file \"%s\"\n\n", outputfile );
                    //return -2;

                }
                else
                {
                    (*outputflag) = 0;
                    printf("\nWeird error with file \"%s\"\n\n", outputfile );     
                }

            }

            if( (*outputflag) > 0 )
                fclose(foutmp);

        }
        else if (!(strcmp(argv[i], "-d")))
        {
            if (str_isdigit(argv[i+1]))
            {
                int tempmetric = atoi(argv[i+1]);
                if (tempmetric!=1 && tempmetric!=2)
                {
                    printf("\n\"d(metric)\" must be 1(euclidean) or 2(cosine)!\n");
                    //return -11;
                    (*metric) = 1;//default is euclidean
                    printf("\nDefault value of metric was chosen (metric: euclidean)\n");
                }
                else
                {
                	(*metric) = tempmetric;
                }
            }
            else
            {
                printf("\n\"d(metric)\" must be a digit!\n");
            }
            
        }
        else if(!(strcmp(argv[i], "-complete")))
        {
            (*complete_print) = 1;
        }
        
    }

    //assigning default values if not given!
    if ((*metric) == -9)
    {
    	(*metric) = 1; //default is euclidean
    }

    if(!(*inputflag) || !(*configflag) || !(*outputflag))
    {
    	printf("\nError\nSomething went wrong with the files!\n");
    	fprintf(stderr,"\nUsage: %s -i <input_file> -c <configuration_file> -o <output_file> -d <metric>\n\n", argv[0]);
        return -99;
    }

    read_config_file(clusters_num, hfuns_num, hashtables_num, configfile);

    if((*clusters_num) == -9)
    {
    	(*clusters_num) = default_cluster_num;
    }

    if((*hfuns_num) == -9)
    {
    	(*hfuns_num) = 4;
    }

    if((*hashtables_num) == -9)
    {
    	(*hashtables_num) = 5;
    }


    printf("\nNumber of clusters: %d\n",  (*clusters_num));
    printf("\nNumber of h functions: %d\n",  (*hfuns_num));
    printf("\nNumber of hashTables: %d\n",  (*hashtables_num));
    printf("\nMetric: ");
    if((*metric) == 1 )
    {
    	printf("euclidean\n");
    }
    else if((*metric) == 2 )
    {
    	printf("cosine\n");
    }

	return 999;

}


int choose_algos(int* init, int* assign, int* update)
{
	char* input = NULL;
	size_t len = 0;
    ssize_t linelen = 0;

	int initflag = 0;
	int assignflag = 0;
	int updateflag = 0;
	int choose = 0;

	printf("\n\nChoose the algorithms of clustering?(y/n)\n");
    printf("(type \"exit\" for exiting the program)\n:");

	if ((linelen = getline(&input, &len, stdin)) == -1)
    {   perror("\nGetline\n");  }

    *(input+linelen-1) = '\0';

    if(!strcmp(input, "exit"))//, strlen("exit")))
    {
        free(input);
        return -99;
    }
    else if(!strcmp(input, "n"))
    {
    	free(input);
    	return 0;
    }
    else if(!strcmp(input, "y"))
    {
    	choose = 1;
    }

    while ((input == NULL) || !choose)
    {

    	printf("\n\nChoose the algorithms of clustering?(y/n)\n");
	    printf("(type \"exit\" for exiting the program)\n:");

		if ((linelen = getline(&input, &len, stdin)) == -1)
	    {   perror("\nGetline\n");  }

	    *(input+linelen-1) = '\0';

	    if(!strcmp(input, "exit"))//, strlen("exit")))
	    {
	        free(input);
	        return -99;
	    }
	    else if(!strcmp(input, "n"))
	    {
	    	free(input);
	    	return 0;
	    }
	    else if(!strcmp(input, "y"))
	    {
	    	choose = 1;
	    }
	    

    }


    while ((input == NULL) || (!initflag) )
    {

    	if(!initflag)
    	{

    		printf("\n\nChoose Initialization algorithm\n1. Random selection of k points\n2. K-means++\n");
   	 		printf("(type \"exit\" for exiting the program)\n:");

   	 		if ((linelen = getline(&input, &len, stdin)) == -1)
		    {   perror("\nGetline\n");  }

		    *(input+linelen-1) = '\0';

		    if(!strcmp(input, "exit"))//, strlen("exit")))
		    {
		        free(input);
		        return -99;
		    }
		    else if(!strcmp(input, "1"))
		    {
		    	(*init) = 1;
		    	initflag = 1;
		    }
		    else if(!strcmp(input, "2"))
		   	{
		   		(*init) = 2;
		   		initflag = 1;
		   	}
		   	else
		   	{
		   		initflag = 0;
		   	}


    	}
  
       
    }

    while ((input == NULL) || (!assignflag) )
    {

    	if(!assignflag)
    	{

    		printf("\n\nChoose Assignment algorithm\n1. Lloyd’s assignment\n2. Assignment by Range search with LSH\n3. Assignment by Range search with Hypercube\n");
   	 		printf("(type \"exit\" for exiting the program)\n:");

   	 		if ((linelen = getline(&input, &len, stdin)) == -1)
		    {   perror("\nGetline\n");  }

		    *(input+linelen-1) = '\0';

		    if(!strcmp(input, "exit"))//, strlen("exit")))
		    {
		        free(input);
		        return -99;
		    }
		    else if(!strcmp(input, "1"))
		    {
		    	(*assign) = 1;
		    	assignflag = 1;
		    }
		    else if(!strcmp(input, "2"))
		   	{
		   		(*assign) = 2;
		   		assignflag = 1;
		   	}
		   	else if(!strcmp(input, "3"))
		   	{
		   		(*assign) = 3;
		   		assignflag = 1;
		   	}
		   	else
		   	{
		   		assignflag = 0;
		   	}


    	}
  
       
    }

    while ((input == NULL) || (!updateflag) )
    {

    	if(!updateflag)
    	{

    		printf("\n\nChoose Update algorithm\n1. K-means\n2. Partitioning Around Medoids (PAM) improved like Lloyd’s\n");
   	 		printf("(type \"exit\" for exiting the program)\n:");

   	 		if ((linelen = getline(&input, &len, stdin)) == -1)
		    {   perror("\nGetline\n");  }

		    *(input+linelen-1) = '\0';

		    if(!strcmp(input, "exit"))//, strlen("exit")))
		    {
		        free(input);
		        return -99;
		    }
		    else if(!strcmp(input, "1"))
		    {
		    	(*update) = 1;
		    	updateflag = 1;
		    }
		    else if(!strcmp(input, "2"))
		   	{
		   		(*update) = 2;
		   		updateflag = 1;
		   	}
		   	else
		   	{
		   		updateflag = 0;
		   	}

    	}
      
    }

    free(input);

	return 9;
}

//like the "readfilemode" from part1 of the project
struct multipoints* save_points(struct multipoints* list, char *filename, point_num_type *totalpoints, int dimension_of_coords)
{
	if(print_msgs_on == 1)        
        printf("\n> Reading from input file \"%s\"...\n", filename);
    
    FILE* fp = NULL ;
    
    if ((fp = fopen(filename, "r")) == NULL)
    {
        perror("\nfopen input-file\n");
        return NULL;
    }
    
    size_t len = 0;
    ssize_t linelen = 0;
    char delimit[]=", \n\t";
    
    char *token = NULL;
    int i = 0;
    struct multipoints* templist = NULL;

    templist = list;
    
    while(!feof(fp))
    {

        char *temp = NULL;
        char *ttemp = NULL;
        
        linelen = getline(&temp, &len, fp);  
        ttemp = temp;

        if ( strlen(temp)<1 || temp == NULL )
        {
            printf("\nWarning: Empty line detected!\n");
        }    
        else
        {
            //printf("\ntempline: %s", temp);
            *(temp+linelen-1) = '\0';
            
            point_num_type y = 0;
            while (temp !=NULL && (token = strtok_r(temp, delimit, &temp))!=NULL)
            {
                
                coordtype k = -9;
                
                if (y == 0)
                {
                    char *name = NULL;
                    int namesize = strlen(token)+1;
                    name = malloc(namesize*sizeof(char));
                    memset((void*)name, 0, namesize*sizeof(char));

                    
                    strncpy(name, token, namesize-1);
                    *(name+namesize-1) = '\0';

                    templist = multiPointList_insertpoint(templist, name, dimension_of_coords);
                   
                }
                else if (y<d+1)
                {       
                    //printf("\nCoord:%s", token);
                    k = atof(token);
                    multiPointList_insertxcoord(templist, k, y-1);

                    
                }

                y++;
                
            }

            
            (*totalpoints)++;
           
        }
        
        
        i++;
        free(ttemp);
        ttemp = NULL;
        //free(temp);
        temp = NULL;
        len = 0;
    }

    fclose(fp);
    
    return list;

}

//print to output file
int print_line_file(FILE* outfd)
{

    struct winsize win2;
    ioctl(0, TIOCGWINSZ, &win2);
    int width = (win2.ws_col);
    fprintf(outfd, "\n");
    for (int f = 0; f < width; f++)
    {   fprintf(outfd, "-"); }

    return 33;

}

int print_to_outputfile(int init, int assign, int update, char* outputfile, struct multipoints* MpointsList, point_num_type totalpoints_in, struct cluster* clusters_ptr[], int clusters_num, int metric, double silhouette_array[], double clustering_time, int complete_print)
{
    point_num_type totpoints_in_clusters = 0;

    FILE *outfd = NULL;
    if ((outfd = fopen(outputfile, "a")) == NULL)
    {
        perror("\nfopen output-file\n");
        return -99;
    }


    fprintf(outfd, "Algorithm: %dx%dx%d\n", init, assign, update );
    fprintf(outfd, "\nMetric: " );
    if(metric == 1)
    {
        fprintf(outfd, "Euclidean\n\n" );
    }
    else if(metric == 2)
    {
        fprintf(outfd, "Cosine\n\n" );
    }

    for (int i = 0; i < clusters_num; i++)
    {

        fprintf(outfd, "CLUSTER-%d ", i );
        fprintf(outfd, "{ size: %lld, ", get_points_in_cluster(i, clusters_ptr));
        totpoints_in_clusters += get_points_in_cluster(i, clusters_ptr);
        fprintf(outfd, "centroid: ");
        char* centroidname = get_name_multipoints(get_cluster_centroid(i, clusters_ptr));
        if(!(strncmp(centroidname, "Mean", strlen("Mean"))))
        {
            for(int l = 0; l < d; l++)
            {
                fprintf(outfd, "%lf ", get_coord_multipoints(get_cluster_centroid(i, clusters_ptr), l) );
            }
        }
        else
        {
            fprintf(outfd, "%s ", get_name_multipoints( get_cluster_centroid(i, clusters_ptr) ) );
        }
        fprintf(outfd, "}\n\n");

    }

    clustering_time = (double)clustering_time / ((double)1000);
    fprintf(outfd, "clustering_time: %lf\n", clustering_time);

    fprintf(outfd, "\nSilhouette: [ ");
    for (int p = 0; p < clusters_num; p++)
    {
        fprintf(outfd, "%lf ", silhouette_array[p]);
    }
    fprintf(outfd, "%lf ]\n", silhouette_array[clusters_num] );

    
    if(complete_print)
    {
        fprintf(outfd, "\n");
        for (int i = 0; i < clusters_num; i++)
        {

            fprintf(outfd, "\nCLUSTER - %d {", i );
            struct points_in_cluster* templist = NULL;
            templist = get_firstpoint_in_cluster(i, clusters_ptr);
            while (templist!=NULL)
            {
                fprintf(outfd, " %s", get_name_multipoints(get_point_points_in_cluster(templist)) );

                templist = get_next_points_in_cluster(templist);
            }
            fprintf(outfd, " }\n" );



        }

    }

    //printf("Total Points: %lld", totpoints_in_clusters);

    fprintf(outfd, "\n\n" );

    fclose(outfd);

    return 24;

}


//evaluation
double dist_average(int k, struct cluster* clusters_ptr[], struct points_in_cluster* templist, int metric, int dimension_of_coords)
{
    double x = 0.0;
    coordtype tempx = 0.0;
    struct points_in_cluster* centroid = NULL;
    centroid = get_firstpoint_in_cluster(k, clusters_ptr);
    while(centroid!=NULL)
    {

        tempx = get_dist_multipoints(get_point_points_in_cluster(centroid), get_point_points_in_cluster(templist), metric, dimension_of_coords);
        tempx = (double)tempx / ((double)get_points_in_cluster(k, clusters_ptr));

        x += tempx;

        centroid = get_next_points_in_cluster(centroid);
    }

    return x;
}

int second_closest_centroid(int k, struct cluster* clusters_ptr[], int clusters_num, struct points_in_cluster* templist, int metric, int dimension_of_coords)
{
    int sec_closest_centr = -9;

    coordtype tempdist = 0.0, mindist = 0.0;
    for(int y = 0; y < clusters_num; y++) //all the centroids so far
    {   

        if (y!=k) //if not the centroid of cluster that i examine its points
        {
            tempdist = get_dist_multipoints(get_cluster_centroid(y, clusters_ptr), get_point_points_in_cluster(templist), metric, dimension_of_coords);

            if(!mindist)
            {
                mindist = tempdist;
                sec_closest_centr = y;
            }
            else
            {
                if(tempdist < mindist)
                {
                    mindist = tempdist;
                    sec_closest_centr = y;
                }
            }
        }
    }


    return sec_closest_centr;
}

//this function is creating the array consisting of the averages of averages of each point's silhouette in a cluster and in the whole dataset(formula given in slides)
int silhouette(struct multipoints* MpointsList, point_num_type totalpoints_in, struct cluster* clusters_ptr[], int clusters_num, double silhouette_array[], int metric, int dimension_of_coords)
{   

    double final_result_i = 0.0, final_result_tot = 0.0, a = 0.0, b = 0.0, max = 0.0;
    struct points_in_cluster* templist = NULL;

    //as in PAM , I tried to quicken the silhouette proceedure by adding a distances array for the points inside the cluster
    for(int k = 0; k < clusters_num; k++)
    {

        //save the distances to make the algorithm quicker
        double **distarray = NULL;
        point_num_type points_of_cluster = get_points_in_cluster(k, clusters_ptr);
        distarray = malloc(sizeof(double *) * points_of_cluster);
        for (point_num_type x = 0; x < points_of_cluster ; x++)
        {
            distarray[x] = malloc(sizeof(double) * (x+1));

            for (point_num_type y = 0; y <= x ; y++)
            {

                distarray[x][y] = -9;
            }

        }



        printf(" .");
        templist = get_firstpoint_in_cluster(k, clusters_ptr);
        
        //for each point, I use the formula and create their silhouette
        point_num_type x = 0, y = 0;
        while(templist!=NULL)
        {

            coordtype tempa = 0.0;
            struct points_in_cluster* centroid1 = NULL;
            centroid1 = get_firstpoint_in_cluster(k, clusters_ptr);

            //creating the formula given in slides
            int same  = 0;
            //average distance from all the points in cluster
            while(centroid1!=NULL)
            {
                if(!(same = check_if_same(get_point_points_in_cluster(centroid1), get_point_points_in_cluster(templist), dimension_of_coords)))
                {
                    if(x >= y)
                    {
                        
                        if( distarray[x][y] == -9) //if the distance hasn't been saved yet, save it
                        {
                            distarray[x][y] = get_dist_multipoints(get_point_points_in_cluster(centroid1), get_point_points_in_cluster(templist), metric, dimension_of_coords);
                            
                        }

                        tempa = distarray[x][y];

                    }
                    else
                    {
                        if( distarray[y][x] == -9) //if the distance hasn't been saved yet, save it
                        {
                            distarray[y][x] = get_dist_multipoints(get_point_points_in_cluster(centroid1), get_point_points_in_cluster(templist), metric, dimension_of_coords);
                            
                        }

                        tempa = distarray[y][x];

                    }

                    tempa = (double)tempa / ((double)get_points_in_cluster(k, clusters_ptr));

                    a += tempa;
                }

                centroid1 = get_next_points_in_cluster(centroid1);
                y++;
            }

            int sec_closest_centr = -9;
            sec_closest_centr = second_closest_centroid(k , clusters_ptr, clusters_num, templist, metric, dimension_of_coords);
            
            coordtype tempb = 0.0;
            struct points_in_cluster* centroid2 = NULL;
            centroid2 = get_firstpoint_in_cluster(sec_closest_centr, clusters_ptr);
            //average distance from all the points in the second closest cluster
            while(centroid2!=NULL)
            {

                tempb = get_dist_multipoints(get_point_points_in_cluster(centroid2), get_point_points_in_cluster(templist), metric, dimension_of_coords);
                tempb = (double)tempb / ((double)get_points_in_cluster(sec_closest_centr, clusters_ptr));

                b += tempb;

                centroid2 = get_next_points_in_cluster(centroid2);
            }

            //printf("\nSecond Closest Centroid(position): %d\n", sec_closest_centr );

            if(a > b)
            {
                max = a;

            }
            else
            {
                max = b;

            }

            coordtype temp_final_result_i = 0.0;
            if(max!=0)
            {
                temp_final_result_i = (double)(b - a)/((double)max);
            }
            else
            {
                temp_final_result_i = 0.0;
            }

            final_result_i += (double)temp_final_result_i / ((double)get_points_in_cluster(k, clusters_ptr));
            final_result_tot += (double)temp_final_result_i / ((double)totalpoints_in);

            templist = get_next_points_in_cluster(templist);
            a = 0.0;
            b = 0.0;
            temp_final_result_i = 0.0;

            x++;
            y = 0;
            
        }
        
        //average of the cluster's points
        silhouette_array[k] += final_result_i;  
        
        //final_result_tot += (double)silhouette_array[k] / ((double)clusters_num);

        //average of the averages - average of all points
        // silhouette_array[clusters_num] += final_result_tot;  

        final_result_i = 0.0;
        //final_result_tot = 0.0;

        //free distances array
        for (point_num_type x = 0; x < points_of_cluster ; x++)
        {
            free(distarray[x]);
            distarray[x] = NULL;
        }

        free(distarray);
        distarray = NULL;

    }

    silhouette_array[clusters_num] = final_result_tot; 

    printf("\n");

    return 19;
}

void print_silhouette(double silhouette_array[], int clusters_num)
{   
    print_msg_centre_line(" Silhouette ");
    printf("\n");

    for (int g = 0; g < clusters_num; g++)
    {
        printf("\ns(%d): %f\n", g, silhouette_array[g] );
    }

    printf("\ns(total): %f\n", silhouette_array[clusters_num] );
}


//overall results for the 12 combinations
struct results
{
    char algo[30];  
    double silhouette;
    double clusterin_time;

};

struct results* results_new(void)
{

    struct results* newnode = NULL;

    newnode = malloc(sizeof(struct results));

    return newnode;
}

void save_algo_results(struct results* tempnode, const char* algo_choice)
{   strcpy(tempnode -> algo, algo_choice); }

void save_silhouette_results(struct results* tempnode, double value)
{   tempnode -> silhouette = value; }

void save_clusterin_time_results(struct results* tempnode, double clustering_time)
{   
    clustering_time = (double)clustering_time/(double)1000;
    tempnode -> clusterin_time = clustering_time; 
}

void print_stats(struct results* array[], int tot_algo_num, int clusters_num, int hfuns_num, int hashtables_num, int metric)
{
    //create a results file to save the stats
    FILE* resultsfd = NULL;

    //get date for file's name    
    time_t ttime = time(NULL);
    struct tm *tm = localtime(&ttime);
    char date[20];
    strftime(date, sizeof(date), "%d-%m-%Y_%H:%M:%S", tm);
    //char filename[40];
    char *filename = NULL;
    int filenamesize = strlen("./results_01-12-2018_21:06:02.txt") + 1;
    filename = malloc(sizeof(char) * filenamesize);
    memset((void*)filename, 0, filenamesize*sizeof(char));

    strcpy(filename, "./results_");
    strcat(filename, date);
    strcat(filename, ".txt");

    

    //printf("\nFilename: %s\n", filename );
    resultsfd = fopen(filename, "w");

    printf("\n");
    print_msg_centre_line("Results");

    printf("Clusters: %d\n", clusters_num);
    fprintf(resultsfd, "Clusters: %d\n", clusters_num);
    
    printf("H functions: %d\n", hfuns_num);
    fprintf(resultsfd, "H functions: %d\n", hfuns_num);
    
    printf("Metric: ");
    fprintf(resultsfd, "Metric: ");

    if(metric == 1)
    {
        printf("Euclidean\n");
        fprintf(resultsfd, "Euclidean\n" );    

        printf("W: %d/%d\n", w, helpful_var );
        fprintf(resultsfd, "W: %d/%d\n", w, helpful_var );

    }
    else if (metric == 2)
    {
        printf("Cosine\n");
        fprintf(resultsfd, "Cosine\n");
    }

    printf("\n> LSH\n");
    fprintf(resultsfd, "\n> LSH\n");

    printf("HashTables: %d\n", hashtables_num);
    fprintf(resultsfd, "HashTables: %d\n", hashtables_num);

    printf("Tablesize: n/%d\n", tablesize_division_factor);
    fprintf(resultsfd, "Tablesize: n/%d\n", tablesize_division_factor);

    printf("\n> hyperCUBE\n");
    fprintf(resultsfd, "\n> hyperCUBE\n");

    printf("Probes(max): %d\n", probes_num );
    fprintf(resultsfd, "Probes(max): %d\n", probes_num );

    printf("Points_to_check(max): %d\n", max_points_to_check);
    fprintf(resultsfd, "Points_to_check(max): %d\n", max_points_to_check);

    printf("\n");
    fprintf(resultsfd, "\n\n");

    printf("Algorithm");
    fprintf(resultsfd, "Algorithm");
    
    for (int k = 0; k < strlen(array[0] -> algo) + 10 - strlen("Algorithm"); k++)
    {   
        printf(" "); 
        fprintf(resultsfd, " "); 
    }

    printf("Silhouette");
    fprintf(resultsfd, "Silhouette");

    for (int k = 0; k < sizeof(double) + 10 - strlen("Silhouette"); k++)
    {   
        printf(" "); 
        fprintf(resultsfd, " "); 

    }

    printf("Time(sec)\n\n");
    fprintf(resultsfd, "Time(sec)\n\n");

    for (int i = 0; i < tot_algo_num; i++)
    {
        printf("%s", array[i] -> algo);
        fprintf(resultsfd, "%s", array[i] -> algo);

        int spaces = 10;
        if((array[i] -> silhouette) < 0)
        {
            spaces = 9;
        }
        for (int k = 0; k < spaces; k++)
        {   
            printf(" "); 
            fprintf(resultsfd, " "); 

        }

        printf("%lf", array[i] -> silhouette);
        fprintf(resultsfd, "%lf", array[i] -> silhouette);

        
        for (int k = 0; k < 10; k++)
        {   
            printf(" "); 
            fprintf(resultsfd, " "); 

        }

        printf("%lf\n", array[i] -> clusterin_time);
        fprintf(resultsfd, "%lf\n", array[i] -> clusterin_time);

    }

    fprintf(resultsfd, "\nInitialization\n1. Random selection of k points\n2. K-means++\n");
    fprintf(resultsfd, "\nAssignment\n1. Lloyd’s assignment\n2. Range search with LSH\n3. Range search with Hypercube\n");
    fprintf(resultsfd, "\nUpdate\n1. K-means\n2. Partitioning Around Medoids (PAM) improved like Lloyd’s\n\n");

    fclose(resultsfd);

    delete_results(array, tot_algo_num);

}

void print_results_position(struct results* compare_results[], int pos)
{       
    printf("\n");

    printf("%s|%lf|%lf", compare_results[pos] -> algo, compare_results[pos] -> silhouette, compare_results[pos] -> clusterin_time );

    printf("\n");
}

//free results structure
void delete_results(struct results* array[], int size)
{

    for(int i = 0; i < size; i++)
    {
        free(array[i]);
        array[i] = NULL;
    }

}


