//
//  recommend.c
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#include "recommend.h"

//cosine LSH 1_1
int cosine_LSH_recom_1_1(struct users* users[], long long users_num, char* outputfile, int dimension_of_coords, int p_nearest, struct crypto_coins* coins[], int coins_num)
{
   
    struct simple_list* p_neighbs = NULL;
    struct multipoints* Mpointlist_users = NULL;
    Mpointlist_users = multiPointList_init(dimension_of_coords);

    //copy users to an Mpointlist to fit the structure for lsh range search
    users_to_Mpointlist(users, users_num, Mpointlist_users, dimension_of_coords);
    //printf("\n%d", __LINE__);
    //printList(Mpointlist_users, dimension_of_coords);
    //printf("\n%d", __LINE__);
    
    //lsh - intro
    int k = hfuns_num_def;
    int L = hashtables_num_def;
    int r[hfuns_num_def]; memset((void*)r, 0, hfuns_num_def*sizeof(int));
	struct hfun *hfarray[hashtables_num_def][hfuns_num_def];
	struct bucket *hashtablesarray[hashtables_num_def];
	point_num_type tablesize = 0;
    
    //cosine lsh
    if(print_msgs_on == 1)
        printf("\nlsh_range");
    
    if(print_msgs_recom_on == 1)
        printf("\nCreating LSH structures\n");
    lsh_range(Mpointlist_users, users_num, k, L, metric_lsh_def, r, hfarray, hashtablesarray, &tablesize , dimension_of_coords);
    
    if(print_msgs_recom_on == 1)
        printf("\nRunning LSH algo\n");
    lsh_p_range(p_neighbs, Mpointlist_users, users_num, k, L, tablesize, hashtablesarray, hfarray, r, metric_lsh_def, dimension_of_coords, p_nearest, users, users_num, coins_num, 11);
    //printf("\n%d", __LINE__);

    //find the best
    find_best(users, users_num, coins, dimension_of_coords, 11);

    //deletes
    delete_multipointlist(Mpointlist_users);
    delete_all_lsh(k, L, hfarray, tablesize, hashtablesarray, metric_lsh_def);

    
    //printf("\n\nLalala --  -- -- --");
  

    return 22;
}

//cosine LSH 1_2
int cosine_LSH_recom_1_2(struct users* users_representatives[], long long users_representatives_num, struct users* users[], long long users_num, char* outputfile, int dimension_of_coords, int p_nearest, struct crypto_coins* coins[], int coins_num)
{
   
    struct simple_list* p_neighbs = NULL;
    //real users' list
    struct multipoints* Mpointlist_users = NULL;
    Mpointlist_users = multiPointList_init(dimension_of_coords);

    //virtual users' list
    struct multipoints* Mpointlist_users_represent = NULL;
    Mpointlist_users_represent = multiPointList_init(dimension_of_coords);

    //copy users to an Mpointlist to fit the structure for lsh range search
    users_to_Mpointlist(users, users_num, Mpointlist_users, dimension_of_coords);
    users_to_Mpointlist(users_representatives, users_representatives_num, Mpointlist_users_represent, dimension_of_coords);

    //printf("\n%d", __LINE__);
    //printList(Mpointlist_users, dimension_of_coords);
    //printf("\n%d", __LINE__);
    
    //lsh - intro
    int k = hfuns_num_def;
    int L = hashtables_num_def;
    int r[hfuns_num_def]; memset((void*)r, 0, hfuns_num_def*sizeof(int));
	struct hfun *hfarray[hashtables_num_def][hfuns_num_def];
	struct bucket *hashtablesarray[hashtables_num_def];
	point_num_type tablesize = 0;
    
    //cosine lsh
    printf("\nlsh_range");
    // if(print_msgs_recom_on == 1)
    //     printf("\nCreating LSH structures\n");
    lsh_range(Mpointlist_users_represent, users_representatives_num, k, L, metric_lsh_def, r, hfarray, hashtablesarray, &tablesize , dimension_of_coords);
    
    if(print_msgs_recom_on == 1)
        printf("\nRunning LSH algo\n");
    lsh_p_range(p_neighbs, Mpointlist_users, users_representatives_num, k, L, tablesize, hashtablesarray, hfarray, r, metric_lsh_def, dimension_of_coords, p_nearest, users, users_num, coins_num, 12);
    //printf("\n%d", __LINE__);

    //find the best
    find_best(users, users_num, coins, dimension_of_coords, 12);

    //delete structs
    delete_multipointlist(Mpointlist_users);
    delete_multipointlist(Mpointlist_users_represent);
    delete_all_lsh(k, L, hfarray, tablesize, hashtablesarray, metric_lsh_def);


    //printf("\n\nLalala --  -- -- --");
  
  	
    return 22;
}


//clustering 2_1
int clustering_recom_2_1(struct users* users[], long long users_num, char* outputfile, int dimension_of_coords, int p_nearest, struct crypto_coins* coins[], int coins_num, long long tweets_num)
{
    struct multipoints* Mpointlist_users = NULL;
    Mpointlist_users = multiPointList_init(dimension_of_coords);

    //copy users to an Mpointlist to fit the structure for clustering 
    users_to_Mpointlist(users, users_num, Mpointlist_users, dimension_of_coords);
    //printList(Mpointlist_users, dimension_of_coords);
    //create the clusters
    int clusters_num = 0;
    //clusters_num = ceil((double)((double)tweets_num/(double)p_nearest ) );
    clusters_num = clusters_for_recom;
    struct cluster* clusters[1];
    clusters[0] = malloc( clusters_num * get_size_cluster());
    clusters_init(clusters_num, clusters);
    char outfile[40] = "./clusterintemp.txt";
    if(print_msgs_recom_on == 1)
        printf("\nClustering the real Users\n");
    clustering_recommendation(clusters, Mpointlist_users, outfile, 1, 2, 1, 1, dimension_of_coords, users_num, clusters_num );
    //printClusters(clusters, clusters_num);
    //printf("\n%d\n", __LINE__);
    //label users according to their cluster
    label_users_from_clusters(clusters, clusters_num, users);

    //struct points_in_cluster* templist = NULL;
    for (int h = 0; h < clusters_num; h++ )
    {
        struct simple_list* rlist = NULL;

        //templist = get_firstpoint_in_cluster(h, clusters);
        
        //neighbours in cluster - rlist is the whole cluster
        rlist = cluster_to_rlist(clusters, h);
        sort_rlist_p(rlist, p_nearest);
        
        // if(print_msgs_recom_on == 1)
        //     printf("\nUpdating Ratings...\n");
        
        if(rlist!=NULL)
        {   
            //printf("\nCluster: %d", h);
            //print_rlist(rlist);

            struct simple_list* templist = NULL; //querypoint  - every point in the cluster
            struct multipoints* user_to_update = NULL;
            templist = rlist;
            while(templist!=NULL)
            {
                //update ratings of unrated coins
                user_to_update =  get_point_simple_list( templist);
                
                update_ratings(user_to_update, rlist, Mpointlist_users, users, users_num, coins_num, p_nearest, 2, dimension_of_coords, users_num, 21);

                templist = get_next_simple_list(templist);
            
            } 
            // rlist = NULL;
            delete_rlist(rlist);
        }
    }

    //find the best
    find_best(users, users_num, coins, dimension_of_coords, 11);

    //deletes
    delete_clusters(clusters_num, clusters);
    delete_multipointlist(Mpointlist_users);

    // if(print_msgs_recom_on == 1)
    //     printf("\n\nClusterRecomOut\n");

    return 98;

}

//clustering 2_2
int clustering_recom_2_2(struct users* users_representatives[], long long users_representatives_num, struct users* users[], long long users_num, char* outputfile, int dimension_of_coords, int p_nearest, struct crypto_coins* coins[], int coins_num, long long tweets_num)
{
    struct multipoints* Mpointlist_users = NULL;
    Mpointlist_users = multiPointList_init(dimension_of_coords);

    //copy virtual users to an Mpointlist to fit the structure for clustering 
    users_to_Mpointlist(users_representatives, users_representatives_num , Mpointlist_users, dimension_of_coords);
    
    //create the clusters
    int clusters_num = ceil((double)((double)tweets_num/(double)p_nearest ) );
    struct cluster* clusters[1];
    clusters_num = clusters_for_representatives;
    clusters[0] = malloc( clusters_num * get_size_cluster());
    clusters_init(clusters_num, clusters);
    char outfile[40] = "./clusterintemp.txt";
    
    //save last node of list to have it
    struct multipoints* templist = NULL, *last_node = NULL;
    templist = Mpointlist_users;
    while(templist!=NULL)
    {   
        last_node = templist;  
        templist = get_next_multipoints(templist);
    }

    char* tempname = NULL;
    struct multipoints* querypointnode = NULL;
    for(long long u = 0; u < users_num; u++)
    {
        
        //add user to the list with virtual users
        querypointnode = multiPointList_init(dimension_of_coords);  
        tempname = malloc(sizeof(long long)*sizeof(char));
        //sprintf(tempname, "%lld", get_user_id_users(users, u));
        sprintf(tempname, "%lld", u+clusters_num);
        //distinguish from the rest of the points
        save_real_multipoints(querypointnode, 1);
        //printf("\nUserid: %lld , str: %s", get_user_id_users(users, u), tempname);
        //printf("\n%d\n", __LINE__);
        multiPointList_insertpoint(querypointnode, tempname, dimension_of_coords );
        //printf("\n%d\n", __LINE__);
        for(int k = 0; k < dimension_of_coords; k++)
        {
            multiPointList_insertxcoord(querypointnode, get_user_crypto_rating_users(users, u, k), k);
        } 
        save_next_multipoints(last_node, querypointnode);
        //printList(Mpointlist_users, dimension_of_coords);
        //printf("\nUpdating point: %lld / %lld\n", u, users_num -1);
        //print_point(querypointnode, dimension_of_coords);
        //print_point(last_node, dimension_of_coords);
        
        int clusters_to_make = clusters_num / 4;
        long long tot_points_in = clusters_num + 1; //clusters (virtual users ) + the one querypoint
        // if(print_msgs_recom_on == 1)
        //     printf("\nClustering the virtuals with user: %lld / %lld\n", u, users_num-1);
        clustering_recommendation(clusters, Mpointlist_users, outfile, 1, 2, 1, 1, dimension_of_coords, tot_points_in, clusters_to_make );
        //printClusters(clusters, clusters_num);
        
        //label users according to their cluster
        //label_users_from_clusters(clusters, clusters_num, users);
        
        //get user's cluster
        //printf("\n%d\n", __LINE__);
        int users_cluster = get_centroid_multipoints(querypointnode);
        
        // if(print_msgs_recom_on == 1)
        //     printf("User's cluster: %d", users_cluster);

        //update rating
        //struct points_in_cluster* templist = NULL;
        //templist = get_firstpoint_in_cluster(users_cluster, clusters);

        struct simple_list* rlist = NULL;
        rlist = cluster_to_rlist(clusters, users_cluster);
        sort_rlist_p(rlist, p_nearest);
        //neighbours in cluster - rlist is the whole cluster without the real user- querypoint
        // if(print_msgs_recom_on == 1)
        //     printf("\nUpdating Ratings...\n");
        if(rlist!=NULL)
        {   
            //printf("\nCluster: %d", h);
            //print_rlist(rlist);

            struct simple_list* templist = NULL; //querypoint  - every point in the cluster
            struct multipoints* user_to_update = NULL;
            templist = rlist;
            // while(templist!=NULL)
            // {
                //update ratings of unrated coins
                //user_to_update =  get_point_simple_list( templist);
           
            
            update_ratings(querypointnode, rlist, Mpointlist_users, users, users_num, coins_num, p_nearest, 2, dimension_of_coords, clusters_num, 22);

               // templist = get_next_simple_list(templist);
            
            // } 
            // rlist = NULL;
            delete_rlist(rlist);
        }

        //delete the structure if we are at the last user
        if(u==(users_num-1))
        {
            delete_clusters(clusters_to_make, clusters);
            //clear_clusters(clusters, clusters_to_make);
        }

        //clear_clusters(clusters, clusters_to_make);

        //remove user from list
        multiPointList_delete_next_node(last_node);
        querypointnode = NULL;
        

    }
    

    // struct points_in_cluster* templist = NULL;
    // for (int h = 0; h < clusters_num; h++ )
    // {
    //     struct simple_list* rlist = NULL;

    //     templist = get_firstpoint_in_cluster(h, clusters);

    //     //neighbours in cluster - rlist is the whole cluster
    //     rlist = cluster_to_rlist(clusters, h);
    //     if(rlist!=NULL)
    //     {   
    //         //printf("\nCluster: %d", h);
    //         //print_rlist(rlist);

    //         struct simple_list* templist = NULL; //querypoint  - every point in the cluster
    //         struct multipoints* user_to_update = NULL;
    //         templist = rlist;
    //         while(templist!=NULL)
    //         {
    //             //update ratings of unrated coins
    //             user_to_update =  get_point_simple_list( templist);
    //             update_ratings(user_to_update, rlist, Mpointlist_users, users, users_num, coins_num, p_nearest);

    //             templist = get_next_simple_list(templist);
            
    //         } 
    //         // rlist = NULL;
    //         delete_rlist(rlist);
    //     }
    // }

    //find the best
    find_best(users, users_num, coins, dimension_of_coords, 22);

    //deletes
    delete_multipointlist(Mpointlist_users);
    
    if(print_msgs_recom_on == 1)
        printf("\n\nClusterRecomOut\n");

    return 98;

}

//helpful functions
int users_to_Mpointlist(struct users* users[], long long users_num, struct multipoints* Mpointlist, int dimension_of_coords)
{
    struct multipoints* templist = NULL;
    templist = Mpointlist;

    for (long long u = 0; u < users_num; u++)
    {
            
        char* tempname = NULL;
        tempname = malloc(sizeof(long long)*sizeof(char));
        //sprintf(tempname, "%lld", get_user_id_users(users, u));
        sprintf(tempname, "%lld", u);
        //printf("\nUserid: %lld , str: %s", get_user_id_users(users, u), tempname);
        templist = multiPointList_insertpoint(templist, tempname, dimension_of_coords );
        //printf("\n%d", __LINE__);
        for(int k = 0; k < dimension_of_coords; k++)
        {
            multiPointList_insertxcoord(templist, get_user_crypto_rating_users(users, u, k), k);
            
        } 
        //printf("\n%d", __LINE__);
        

    }
    //printf("\n%d", __LINE__);



    return 34;
}

int update_ratings(struct multipoints* querypointnode, struct simple_list* rlist, struct multipoints* Mpointlist, struct users* users[], long long users_num, int coins_num, int p_nearest, int metric, int dimension_of_coords, int virtual_users, int prob)
{
    //printf("\nUpdating user%s\n", get_name_multipoints(querypointnode));
    int counter = 0;
    long long neigh = -9;
    double rating_temp = 0.0;
    long long user_index = -9, user_id = -9, temp_user_index = -9;
    double rating_sum = 0.0, final_rating = 0.0, z_factor = 0.0, rating_sum_absol = 0.0, rating_sum_absol_2 = 0.0;
    struct simple_list* templist_simple_list = NULL;
    

    temp_user_index = atoi(get_name_multipoints( querypointnode) );

    //case 2_2 - clustering with virtual users
    if( prob == 22 ) //if we have virtual users + clustering method
    {
        temp_user_index -= virtual_users;
    }

    for(int i = 0; i < coins_num; i++)
    {   
       
        int go_on = 1;
        rating_temp = check_if_known_coin(users, temp_user_index, i);
        if(!rating_temp) //only if it's unknown i rate it, else , it has a rating
        {   
            templist_simple_list = rlist;
            while( (templist_simple_list!=NULL) && (counter < p_nearest)  )
            {   

                //check if it is the same point - not include it
                neigh = atoi(get_name_multipoints(get_point_simple_list(templist_simple_list)));
                //printf("\npoint: %lld\n", neigh);
                if( prob == 22  )
                {
                    if(neigh > virtual_users-1)
                    {
                        neigh = temp_user_index;
                        go_on = 0;
                    }

                }

                //check if it's the querypointnode
                if(prob == 11)
                {
                    if(neigh == temp_user_index)
                    {
                        go_on = 0;
                    }
                    else
                    {
                        go_on = 1;
                    }

                }
                else if(prob == 12)
                {
                    go_on = 1;  //always proceeds because rlist does not include querypointnode
                }
                else if (prob == 21)
                {
                    if(neigh == temp_user_index)
                    {
                        go_on = 0;
                    }
                    else
                    {
                        go_on = 1;
                    }

                }
                // if(prob == 21)
                //     printf("\n%lld - %lld (go_on = %d)\n", neigh, temp_user_index, go_on);
                //(neigh!=temp_user_index)
                if( go_on == 1) // check for the clustering - rlist in cluster
                {   

                    if(metric == 1)
                    {
            
                        rating_sum += (double)( (double)euclidean_sim(querypointnode, get_point_simple_list(templist_simple_list), dimension_of_coords) * ( (double) get_coord_multipoints(get_point_simple_list(templist_simple_list), i) - get_average_rating_users(users, neigh )));     
                        rating_sum_absol_2 = (double)euclidean_sim(querypointnode, get_point_simple_list(templist_simple_list), dimension_of_coords);
                    

                    }
                    else if(metric == 2)
                    {
                        rating_sum += (double)( (double)cos_v(querypointnode, get_point_simple_list(templist_simple_list), dimension_of_coords) * ( (double) get_coord_multipoints(get_point_simple_list(templist_simple_list), i) - get_average_rating_users(users, neigh )));     
                        rating_sum_absol_2 = (double)cos_v(querypointnode, get_point_simple_list(templist_simple_list), dimension_of_coords);
                    
                    }

                    rating_sum_absol += fabs((double)rating_sum_absol_2);
                    //printf("\n Rating sum: %lf - absol: %lf", rating_sum, rating_sum_absol);

                    counter++; //count only the p_nearest nearest neighbours
                }
                templist_simple_list = get_next_simple_list(templist_simple_list);
                go_on = 1;
            }
            //finalize rating
            if( (!rating_sum_absol) || (!rating_sum) || (rlist==NULL) )
            {      
                final_rating = get_average_rating_users(users, temp_user_index);
                //printf("\nfinal:%lf", final_rating);
            }
            else
            {
                z_factor = (double)( (double)1 / (double)rating_sum_absol);
                final_rating = get_average_rating_users(users, temp_user_index) + (double)z_factor * (double)(rating_sum); 
                //printf("\nz_factor: %lf - final: %lf (rating_sum:%lf)", z_factor, final_rating, rating_sum);
            }
            // printf("\nuser_in: %lld(userid: %lld)-finalr: %lf", temp_user_index, get_user_id_users(users, temp_user_index), final_rating);
            //find the user - from point
            //user_id = atoi(get_name_multipoints(querypointnode));
            //user_index = get_pos_from_id_users(users, users_num, user_id);
            user_index = temp_user_index;
            //printf("\nUsername: |%s| - user id: |%lld| - user_index: |%lld|", get_name_multipoints(querypointnode), user_id, user_index);
            save_user_crypto_rating_users(users, user_index, i, final_rating);
            rating_sum = 0.0;
            rating_sum_absol = 0.0;
        }

        counter = 0;
    }
    

    return 33;
}

struct recom_coins_list
{
    double score;
    int coin_index;
};

int find_min_pos(struct recom_coins_list* temp5best[], int size)
{   
    int min_pos = -9;
    double min_score = 999;

    for (int p = 0; p < size; p++)
    {
        if(!p)
        {
            min_pos = 0;
            min_score = temp5best[p] -> score;
        }
        else
        {
            if ( (temp5best[p] -> score) < min_score )
            {
                min_pos = p;
                min_score = temp5best[p] -> score;
            }
        }

    }

    return min_pos;

}

void find_best(struct users* users[], long long users_num, struct crypto_coins* coins[], int coins_num, int problem)
{   
    // if(print_msgs_recom_on == 1)
    //     printf("\nFind best made it!\n");
    
    double cur_score = -999;
    int min_pos = -9;
    double aver = 0.0, temp_sc_coin = 0.0;
    //printf("\nbest: %d\n", __LINE__);
    if((problem == 11) || (problem == 21) ) //real users
    {
        struct recom_coins_list* temp5best[5];
        for (int o = 0; o < 5; o++)
        {
            temp5best[o] = malloc(sizeof(struct recom_coins_list));
            temp5best[o] -> score = -998;
            temp5best[o] -> coin_index = -9;
        }
        //printf("\nbest: %d\n", __LINE__);

        for(long long u = 0; u < users_num; u++)
        {
            aver = get_average_rating_users(users, u); //if not the average
            
            //printf("\nbest: %d\n", __LINE__);
            int tempcounter = 0;
            for(int i = 0; i < coins_num; i++)
            {
                temp_sc_coin = get_user_crypto_rating_users(users , u, i );
                // if( (temp5best[l]->coin_index) != aver)
                if(!(check_if_known_coin(users, u, i)) && (temp_sc_coin!=aver) ) //we don't want the coins that the user already has
                {
                    //printf("\nbest: %d\n", __LINE__);
                    if(tempcounter < 5)
                    {
                        cur_score = get_user_crypto_rating_users(users,u,i);
                        //printf("\nbest: %d\n", __LINE__);
                        //if(cur_score!=aver)
                        //{    
                            temp5best[tempcounter] -> coin_index = i;
                            temp5best[tempcounter] -> score = cur_score;
                        //}
                    }
                    else
                    {
                        cur_score = get_user_crypto_rating_users(users,u,i); 
                        min_pos = find_min_pos(temp5best, 5);
                        //printf("\nbest: %d (minpos: %d)\n", __LINE__, min_pos);
                        if( (temp5best[min_pos]->score) < cur_score)
                        {
                            temp5best[min_pos] -> coin_index = i;
                            temp5best[min_pos] -> score = cur_score;
                        }

                    }
                    tempcounter++;
                }


            }
            //printf("\nbest: %d\n", __LINE__);
            // save to user
            // for(int l = 0; l < 5; l++)
            // {  
            //     printf("\nScore[%d]: %lf - index: %d\n", l, temp5best[l]->score, temp5best[l]->coin_index );
            // }
            char* name_of_coin = NULL;
            for(int l = 0; l < 5; l++)
            {
                //find the name of the coin
                name_of_coin = name_of_coin_from_pos(coins, coins_num, temp5best[l] -> coin_index);
                //aver = get_average_rating_users(users, u); //if not the average
                // if( (temp5best[l]->coin_index) != aver)
                // {
                //printf("\nbest: %d\n", __LINE__);
                save_nth_best_5_real_users(users, u, l, temp5best[l]->coin_index, name_of_coin);
                //printf("\nbest: %d\n", __LINE__);
                // }
                // else
                // {
                //     save_nth_best_5_real_users(users, u, l, -9, NULL);
                // }
            }

            //printf("\nbest: %d\n", __LINE__);
            //print_best5_nth_user_users(users, u);
            //printf("\nbest: %d\n", __LINE__);


        }

        for(int i = 0; i < 5; i++)
        {    
            free(temp5best[i]);
        }

    }
    else //representative users
    {

        struct recom_coins_list* temp2best[2];
        for (int o = 0; o < 2; o++)
        {
            temp2best[o] = malloc(sizeof(struct recom_coins_list));
            temp2best[o] -> score = -998;
            temp2best[o] -> coin_index = -9;
        }
        //printf("\nbest: %d\n", __LINE__);

        for(long long u = 0; u < users_num; u++)
        {
            aver = get_average_rating_users(users, u); //if not the average
            
            //printf("\nbest: %d\n", __LINE__);
            int tempcounter = 0;
            for(int i = 0; i < coins_num; i++)
            {
                temp_sc_coin = get_user_crypto_rating_users(users , u, i );
                // if( (temp5best[l]->coin_index) != aver)
                if(!(check_if_known_coin(users, u, i)) && (temp_sc_coin!=aver) ) //we don't want the coins that the user already has
                {
                    //printf("\nbest: %d\n", __LINE__);
                    if(tempcounter < 2)
                    {
                        cur_score = get_user_crypto_rating_users(users,u,i);
                        //printf("\nbest: %d\n", __LINE__);
                        //if(cur_score!=aver)
                        //{    
                            temp2best[tempcounter] -> coin_index = i;
                            temp2best[tempcounter] -> score = cur_score;
                        //}
                    }
                    else
                    {
                        cur_score = get_user_crypto_rating_users(users,u,i); 
                        min_pos = find_min_pos(temp2best, 2);
                        //printf("\nbest: %d (minpos: %d)\n", __LINE__, min_pos);
                        if( (temp2best[min_pos]->score) < cur_score)
                        {
                            temp2best[min_pos] -> coin_index = i;
                            temp2best[min_pos] -> score = cur_score;
                        }

                    }
                    tempcounter++;
                }


            }
            //printf("\nbest: %d\n", __LINE__);
            // save to user
            // for(int l = 0; l < 5; l++)
            // {  
            //     printf("\nScore[%d]: %lf - index: %d\n", l, temp5best[l]->score, temp5best[l]->coin_index );
            // }
            char* name_of_coin = NULL;
            for(int l = 0; l < 2; l++)
            {
                //find the name of the coin
                name_of_coin = name_of_coin_from_pos(coins, coins_num, temp2best[l] -> coin_index);
                //aver = get_average_rating_users(users, u); //if not the average
                // if( (temp5best[l]->coin_index) != aver)
                // {
                //printf("\nbest: %d\n", __LINE__);
                save_nth_best_2_represent_users(users, u, l, temp2best[l]->coin_index, name_of_coin);
                //printf("\nbest: %d\n", __LINE__);
                // }
                // else
                // {
                //     save_nth_best_5_real_users(users, u, l, -9, NULL);
                // }
            }

            //printf("\nbest: %d\n", __LINE__);
            //print_best2_nth_user_users(users, u);
            //printf("\nbest: %d\n", __LINE__);


        }

        for(int i = 0; i < 2; i++)
        {    
            free(temp2best[i]);
        }

    }

    //delete structs


}

void label_users_from_clusters(struct cluster* clusters[], int clusters_num, struct users* users[])
{
    struct points_in_cluster* templist = NULL;
    long long user_index = -9;
    for (int i =0; i < clusters_num; i++)
    {
        if((get_points_in_cluster(i, clusters))>0)
        {
            templist = get_firstpoint_in_cluster(i, clusters);
            while(templist != NULL)
            {
                user_index = atoi(get_name_multipoints(get_point_points_in_cluster(templist)));
                
                save_cluster_num_real_users(users, user_index, i);

                templist = get_next_points_in_cluster(templist);
            }

        }


    }
}


struct simple_list* cluster_to_rlist(struct cluster* clusters[], int cluster_pos)
{
    struct simple_list* rlist = NULL, *last_node_rlist = NULL;
    struct points_in_cluster* templist = NULL;
    
    templist = get_firstpoint_in_cluster(cluster_pos, clusters);
    while(templist!=NULL)
    {
        if (rlist == NULL) //first neighbour
        {

            struct simple_list* newnode = NULL;
            newnode = malloc(sizeof(get_size_simple_list()));
            save_next_simple_list(newnode, NULL);
            save_point_simple_list(newnode, get_point_points_in_cluster(templist));
            rlist = newnode;
            last_node_rlist = rlist;

            //neighbour_tot_num++;
        }
        else
        {
            struct simple_list* newnode = NULL;
            newnode = malloc(sizeof(get_size_simple_list()));
            save_next_simple_list(newnode, NULL);
            save_point_simple_list(newnode, get_point_points_in_cluster(templist));
            
            save_next_simple_list(last_node_rlist, newnode);
            last_node_rlist = newnode;

        }

        templist = get_next_points_in_cluster(templist);
    }
    

    return rlist;

}

struct simple_list* cluster_to_rlist_virtual(struct cluster* clusters[], int cluster_pos)
{
    struct simple_list* rlist = NULL, *last_node_rlist = NULL;
    struct points_in_cluster* templist = NULL;
    int is_querypoint = 0;

    templist = get_firstpoint_in_cluster(cluster_pos, clusters);
    while(templist!=NULL)
    {
        is_querypoint = get_real_multipoints(get_point_points_in_cluster(templist));
        if(!is_querypoint)
        {    
            if (rlist == NULL) //first neighbour
            {

                struct simple_list* newnode = NULL;
                newnode = malloc(sizeof(get_size_simple_list()));
                save_next_simple_list(newnode, NULL);
                save_point_simple_list(newnode, get_point_points_in_cluster(templist));
                rlist = newnode;
                last_node_rlist = rlist;

                //neighbour_tot_num++;
            }
            else
            {
                struct simple_list* newnode = NULL;
                newnode = malloc(sizeof(get_size_simple_list()));
                save_next_simple_list(newnode, NULL);
                save_point_simple_list(newnode, get_point_points_in_cluster(templist));
                
                save_next_simple_list(last_node_rlist, newnode);
                last_node_rlist = newnode;

            }
        }

        templist = get_next_points_in_cluster(templist);
    }
    

    return rlist;

}
