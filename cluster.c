//
//  cluster.c
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#include "cluster.h"


int clustering_recommendation(struct cluster* clusters[], struct multipoints* MpointsList, char* outputfile, int metric, int init, int assign, int update, int dimension_of_coords, long long totalpoints_in, int clusters_from_recom)
{	
	//rand seed for later
	struct timeval time; 
    gettimeofday(&time,NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

	// char inputfile[40];
    // memset((void*)inputfile, 0, 40*sizeof(char));
	// int inputflag = 0;

	// char configfile[40];
    // memset((void*)configfile, 0, 40*sizeof(char));
	// int configflag = 0;

	// char outputfile[40];
    // memset((void*)outputfile, 0, 40*sizeof(char));
	// int outputflag = 0;

	//int metric = -9;  //1 for euclidean, 2 for cosine
	int clusters_num = clusters_from_recom;
	int hfuns_num = hfuns_num_def;
	int hashtables_num = hashtables_num_def;

	int complete_print = 0; //if user wants to print all the items of clusters to the outputfile

	//init values!!!!
	//read_config_file(&clusters_num, &hfuns_num, &hashtables_num, configfile);
	// int check_pars = 0;
	// if((check_pars = init_params(argc, argv, &inputflag, inputfile, &configflag, configfile, &outputflag, outputfile, &metric, &clusters_num, &hfuns_num, &hashtables_num, &complete_print))==-99)
	// {
	// 	print_msg_centre_line("CLUSTERIN' OUT!!");
	// 	printf("\n\n");
	// 	return -999;
	// }

	//printf("\nComplete_print : %d\n", complete_print );
	if(print_msgs_on == 1)
		print_welcome_msg();

	// int init = -9; 
	// int assign = -9;
	// int update = -9;
	// int answer = 1;
	// //answer = choose_algos(&init, &assign, &update);  //user input on algorithms - y/n - if y: choose each one

	// if (!answer) //will try all algorithms
	// {	
	// 	print_msg_centre("Starting the process...");
	// 	print_msg_centre_line("12 combinations will be tried out");
	// 	// print_msg_centre_line("will be tried out");

	// }
	// else if(answer == -99)
	// {
	// 	print_msg_centre_line("CLUSTERIN' OUT!!");
	// 	printf("\n\n");
	// 	return -999;
	// }
	// else
	// {
	if(print_msgs_on == 1)
		print_msg_centre("Clusterin: Starting the process...");
	// 	printf("\n");

	// }


	//printf("\n\n");

	

	//struct multipoints* MpointsList = NULL;
	int dimension = dimension_of_coords;
	//printf("\nDimension: %d", dimension);
    //MpointsList = multiPointList_init(dimension); 
    //point_num_type totalpoints_in = 0;
	//save_points(MpointsList, inputfile, &totalpoints_in, dimension );  //save points of input dataset
	//printList(MpointsList);

	//struct cluster* clusters[1];  //the structure of clusters
	//clusters[0] = malloc( clusters_num * get_size_cluster());
	//clusters_init(clusters_num, clusters);  

	FILE* temp = NULL;
	temp = fopen(outputfile, "w+");
	fclose(temp);

	struct results* temp_res = NULL;
	//int dimension_of_coords = d;

	//if (answer > 0) //user's choice
	//{

	temp_res = users_choice(MpointsList, clusters, init, assign, update, outputfile, totalpoints_in, clusters_num, hfuns_num, hashtables_num, metric, complete_print, dimension_of_coords);
	free(temp_res);	//results in case of user's choice don't have a meaning so i free it immediately
	//}
	// else if(!answer) //12 combinations - when the user doesn't choose a certain combination
	// {

	// 	all_combinations(MpointsList, clusters, outputfile, clusters_num, totalpoints_in, hfuns_num, hashtables_num, metric, complete_print, dimension_of_coords);
		
	// }

	//deletes - free memory
	//delete_all(MpointsList, clusters, clusters_num);
	if(print_msgs_on == 1)
		print_msg_centre_line(" CLUSTERin' OUT ! ");
	//printf("\n\n");


	return 999;
}

int clustering(struct cluster* clusters[], char* inputfile, char* configfile, char* outputfile, int metric, int init, int assign, int update, int dimension_of_coords)
{	
	//rand seed for later
	struct timeval time; 
    gettimeofday(&time,NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

	// char inputfile[40];
    // memset((void*)inputfile, 0, 40*sizeof(char));
	// int inputflag = 0;

	// char configfile[40];
    // memset((void*)configfile, 0, 40*sizeof(char));
	// int configflag = 0;

	// char outputfile[40];
    // memset((void*)outputfile, 0, 40*sizeof(char));
	// int outputflag = 0;

	//int metric = -9;  //1 for euclidean, 2 for cosine
	int clusters_num = clusters_for_representatives;
	int hfuns_num = hfuns_num_def;
	int hashtables_num = hashtables_num_def;

	int complete_print = 0; //if user wants to print all the items of clusters to the outputfile

	//init values!!!!
	//read_config_file(&clusters_num, &hfuns_num, &hashtables_num, configfile);
	// int check_pars = 0;
	// if((check_pars = init_params(argc, argv, &inputflag, inputfile, &configflag, configfile, &outputflag, outputfile, &metric, &clusters_num, &hfuns_num, &hashtables_num, &complete_print))==-99)
	// {
	// 	print_msg_centre_line("CLUSTERIN' OUT!!");
	// 	printf("\n\n");
	// 	return -999;
	// }

	//printf("\nComplete_print : %d\n", complete_print );
	if(print_msgs_on == 1)
		print_welcome_msg();

	// int init = -9; 
	// int assign = -9;
	// int update = -9;
	// int answer = 1;
	// //answer = choose_algos(&init, &assign, &update);  //user input on algorithms - y/n - if y: choose each one

	// if (!answer) //will try all algorithms
	// {	
	// 	print_msg_centre("Starting the process...");
	// 	print_msg_centre_line("12 combinations will be tried out");
	// 	// print_msg_centre_line("will be tried out");

	// }
	// else if(answer == -99)
	// {
	// 	print_msg_centre_line("CLUSTERIN' OUT!!");
	// 	printf("\n\n");
	// 	return -999;
	// }
	// else
	// {
	if(print_msgs_on == 1)
		print_msg_centre("Clusterin: Starting the process...");
	// 	printf("\n");

	// }


	printf("\n\n");

	

	struct multipoints* MpointsList = NULL;
	int dimension = dimension_of_coords;
	//printf("\nDimension: %d", dimension);
    MpointsList = multiPointList_init(dimension); 
    point_num_type totalpoints_in = 0;
	save_points(MpointsList, inputfile, &totalpoints_in, dimension );  //save points of input dataset
	//printList(MpointsList);

	//struct cluster* clusters[1];  //the structure of clusters
	//clusters[0] = malloc( clusters_num * get_size_cluster());
	clusters_init(clusters_num, clusters);  

	FILE* temp = NULL;
	temp = fopen(outputfile, "w+");
	fclose(temp);

	struct results* temp_res = NULL;
	//int dimension_of_coords = d;

	//if (answer > 0) //user's choice
	//{

		temp_res = users_choice(MpointsList, clusters, init, assign, update, outputfile, totalpoints_in, clusters_num, hfuns_num, hashtables_num, metric, complete_print, dimension_of_coords);
		free(temp_res);	//results in case of user's choice don't have a meaning so i free it immediately
	//}
	// else if(!answer) //12 combinations - when the user doesn't choose a certain combination
	// {

	// 	all_combinations(MpointsList, clusters, outputfile, clusters_num, totalpoints_in, hfuns_num, hashtables_num, metric, complete_print, dimension_of_coords);
		
	// }

	//deletes - free memory
	//delete_all(MpointsList, clusters, clusters_num);

	if(print_msgs_on == 1)
		print_msg_centre_line(" CLUSTERin' OUT ! ");
	
	printf("\n\n");


	return 999;
}