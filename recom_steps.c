//
//  recom_steps.c
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#include "recom_steps.h"

int recom_steps(struct tweets* tweets[], long long tweets_num, struct sentim_score_map* sentim_map[], long long sentim_map_size, struct crypto_coins* coins[], long long coins_size, struct users* users[], long long users_num, char* outputfile, int p_nearest)
{
    int dimension_of_coords = coins_size;
    
    //sentiment score
    calculate_sentim_score(tweets, tweets_num, sentim_map, sentim_map_size);
    //print_tweets(tweets, tweets_num, coins);

    //create users
    create_users(users, users_num, tweets, tweets_num, coins, coins_size );
    //print_users(users, users_num,coins_size, coins);
    //normalise_vector_user_crypto(users, users_num, coins_size);
    calculate_average_rating(users, users_num, coins_size);
    user_crypto_save_users(users, users_num, dimension_of_coords);
    //print_users(users, users_num,coins_size, coins);
    //print_tweets(tweets, tweets_num, coins);

    //clear users with no proposals
    long long users_with_coins_num = 0;
    users_with_coins_num = count_users_with_coins(users, users_num, coins_size);
    printf("\nUsers with coins: %lld\n", users_with_coins_num);
    
    struct users* users_with_coins[1];
    if(users_with_coins_num!=users_num) //if we have users with no coins
    {        
        users_with_coins[0] = malloc( users_with_coins_num * get_size_users());
        users_init(users_with_coins, users_with_coins_num, coins_size);    

        clear_users_with_no_coins(users, users_num, users_with_coins, users_with_coins_num, coins_size);
        users[0] = delete_users(users, users_num, coins_size);

        users[0] = users_with_coins[0];
        //delete_users(users_with_coins, users_with_coins_num, coins_size);
        users_num = users_with_coins_num;
    }
    user_crypto_save_users(users, users_num, dimension_of_coords);
    //clear_best_arrays(users, users_num);
    //print_users(users, users_with_coins_num, coins_size, coins);


    //create_representative_users
    if(print_msgs_recom_on == 1)
        printf("\nCreating Virtual Users by clustering the tweets (from file) ...\n");
    int clusters_num = 0;
    struct cluster* clusters[1];
    clusters_num = clusters_for_representatives;
    clusters[0] = malloc( clusters_num * get_size_cluster());
    clusters_init(clusters_num, clusters);
    char tweetsfile[40] = "./twitter_dataset_small_v2.csv";
    char configfile[40] = "./cluster.conf";
    char outfile[40] = "./clusterintemp.txt";
    clustering(clusters, tweetsfile, configfile, outfile, 2, 2, 1, 1, d );
    printf("\nTotal Virtual Users: %d\n", clusters_num);
    //printClusters(clusters, clusters_num);

    struct users* users_representatives[1];
    users_representatives[0] = malloc( clusters_num * get_size_users());
    users_init(users_representatives, clusters_num, coins_size);
    create_represent(clusters, clusters_num, users_representatives, tweets, tweets_num, coins, coins_size);
    //delete cluster_structure
    delete_clusters(clusters_num, clusters);
    

    calculate_average_rating(users_representatives, clusters_num, coins_size);
    user_crypto_save_users(users_representatives, clusters_num, dimension_of_coords);
    //print_users(users_representatives, clusters_num, coins_size, coins);
    
    //normalise_vector_user_crypto(users_representatives, clusters_num, coins_size);
    //print_users(users_representatives, clusters_num, coins_size, coins);
    
    //clear representative users with no proposals
    long long users_representatives_with_coins_num = 0, users_representatives_num = clusters_num;
    users_representatives_with_coins_num = count_users_with_coins(users_representatives, clusters_num, coins_size);
    printf("\nVirtual Users with coins: %lld\n", users_representatives_with_coins_num);
    
    struct users* users_representatives_with_coins[1];
    if(users_representatives_with_coins_num!= users_representatives_num) //if we have users with no coins
    { 
        users_representatives_with_coins[0] = malloc( users_representatives_with_coins_num * get_size_users());
        users_init(users_representatives_with_coins, users_representatives_with_coins_num, coins_size);    

        clear_users_with_no_coins(users_representatives, clusters_num, users_representatives_with_coins, users_representatives_with_coins_num, coins_size);
        users_representatives[0] = delete_users(users_representatives, clusters_num, coins_size);

        users_representatives[0] = users_representatives_with_coins[0];
        users_representatives_num = users_representatives_with_coins_num;
    }
    //print_users(users_representatives, users_representatives_num, coins_size, coins);


    struct timespec recom_start, recom_end;
	double recom_time = 0.0;
    print_line();
	print_msg_centre("1.1 Cosine LSH Recommendation (real users)");
    print_line();
    //Reccomendation_1.1 --------------------------------------------------------------------------------------------------------------------------------------
    clock_gettime(CLOCK_MONOTONIC_RAW, &recom_start);
    
    cosine_LSH_recom_1_1( users, users_num, outputfile, dimension_of_coords, p_nearest, coins, coins_size);
    
    // //print_users(users, users_num, coins_size, coins);

    retrieve_saved_user_crypto_ratings_users(users, users_num, coins_size);
    //printf("\nSteps %d\n", __LINE__);
    print_line();
	print_msg_centre("1.2 Cosine_LSH_Recommendation (virtual users)");
    print_line();
    //Recommendation_1.2 --------------------------------------------------------------------------------------------------------------------------------------
    
    cosine_LSH_recom_1_2(users_representatives, users_representatives_num, users, users_num, outputfile, dimension_of_coords, p_nearest, coins, coins_size);
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &recom_end);
    recom_time = (double)((recom_end.tv_sec - recom_start.tv_sec) * 1000000 + (recom_end.tv_nsec - recom_start.tv_nsec) / 1000);
	recom_time = (double)recom_time/1000;

    print_all_results(users, users_num,  recom_time, 1);

    // //print_users(users, users_num, coins_size, coins);
    write_to_outputfile_recom(outputfile, users, users_num, recom_time, 1);
    //printf("\nSteps %d\n", __LINE__);

    //clear the arrays where i save the best coins for recommendation
    clear_users(users, users_num);
    //printf("\nSteps %d\n", __LINE__);
    
    retrieve_saved_user_crypto_ratings_users(users, users_num, coins_size);

    //print_users(users, users_num, coins_size, coins);

    //printf("\nSteps %d\n", __LINE__);
    print_line();
	print_msg_centre("2.1 Clustering_Recommendation (real users)");
    print_line();
    //Recommendation_2.1 --------------------------------------------------------------------------------------------------------------------------------------
    clock_gettime(CLOCK_MONOTONIC_RAW, &recom_start);

    //print_users(users, users_num, coins_size, coins);
    clustering_recom_2_1(users, users_num, outputfile, dimension_of_coords, p_nearest, coins, coins_size, tweets_num);


    //copy saved crypto-user ratings to create new vector at the next recommendation
    //print_users(users, users_num, coins_size, coins);
    retrieve_saved_user_crypto_ratings_users(users, users_num, coins_size);
    //print_users(users, users_num, coins_size, coins);

    //printf("\nSteps %d\n", __LINE__);
    print_line();
	print_msg_centre("2.2 Clustering_Recommendation (virtual users)");
    print_line();
    //Recommendation_2.2 --------------------------------------------------------------------------------------------------------------------------------------
    
    //print_users(users, users_num, coins_size, coins);
    clustering_recom_2_2(users_representatives, users_representatives_num, users, users_num, outputfile, dimension_of_coords, p_nearest, coins, coins_size, tweets_num);

    clock_gettime(CLOCK_MONOTONIC_RAW, &recom_end);
    recom_time = (double)((recom_end.tv_sec - recom_start.tv_sec) * 1000000 + (recom_end.tv_nsec - recom_start.tv_nsec) / 1000);
	recom_time = (double)recom_time/1000;

    //write results of clustering recommendation
    write_to_outputfile_recom(outputfile, users, users_num, recom_time, 2);
   
    //print results
    print_all_results(users, users_num,  recom_time, 2);
    //print_users(users, users_num, coins_size, coins);


    //deleteS --------------------------------------------------------------------------------
    delete_users(users_representatives, users_representatives_num, coins_size);
    
    delete_users(users, users_num, coins_size);
    //delete_users(users_with_coins, users_with_coins_num, coins_size);

    if(print_msgs_recom_on == 1)
        printf("\n\nRecom Steps Out\n");

    return 22;
}


double calculate_sentim_score(struct tweets* tweets[], long long tweets_num, struct sentim_score_map* sentim_map[], long long sentim_map_size)
{
    double tot_score = 0.0, temp_score = 0.0;
    for( long long y = 0; y < tweets_num; y++ )
    {
        struct tweet_content* templist = NULL;
        templist = get_tweet_content_tweets(tweets, y);
        char* tempstr = NULL;
        while(templist!=NULL)
        {

            tempstr = get_word_tweet_content(templist);

            temp_score += get_word_score(sentim_map, sentim_map_size, tempstr);

            templist = get_next_tweet_content(templist);
        }

        //printf(" \n%lf", temp_score);
        tot_score = (double) ( (double)temp_score )/( (double)sqrt( pow(temp_score, 2) + alpha )  );
        //printf("\n%lf", tot_score);
        save_tweet_score_tweets(tweets, y, tot_score);
        temp_score = 0.0;
    }


    return 66;
}

void normalise_vector_user_crypto(struct users* users[], long long users_num, int coins_num)
{
    double temp_score = 0.0;

    for(long long i = 0; i < users_num; i++)
    {
        temp_score = 0;
        for(int t = 0; t < coins_num; t++)
        {
            if((get_user_crypto_rating_users(users, i, t))!=INFINITY)
            {
                temp_score += get_user_crypto_rating_users(users, i, t) ;
            }
        }   
        temp_score = (double)temp_score / (double)(coins_num);
        save_average_rating_users(users,i,temp_score);
        double new_rating = 0;
        for(int t = 0; t < coins_num; t++)
        {
            if((get_user_crypto_rating_users(users, i, t))!=INFINITY)
            {
                new_rating = get_user_crypto_rating_users(users, i, t) - temp_score;
                save_user_crypto_rating_users(users, i, t, new_rating );
            }
            else
            {
                save_user_crypto_rating_users(users, i, t, 0);
            }
        } 

    }


}

void calculate_average_rating(struct users* users[], long long users_num, int coins_num)
{
    int tot_coins_in_user = 0;
    double temp_score = 0.0, cur_score = 0.0;

    for(long long i = 0; i < users_num; i++)
    {
        temp_score = 0;
        for(int t = 0; t < coins_num; t++)
        {
            cur_score = get_user_crypto_rating_users(users, i, t);
            if(cur_score!=INFINITY)
            {                
                temp_score += get_user_crypto_rating_users(users, i, t) ;
            }
        }
        tot_coins_in_user = get_tot_coins_users(users,i);
        if(!tot_coins_in_user)
        {
            temp_score = 0;
            //printf("\nNO COINS");
        }
        else
        {
            temp_score = (double)temp_score / (double)(tot_coins_in_user);
        }
        save_average_rating_users(users,i,temp_score);
        for(int t = 0; t < coins_num; t++)
        {
            cur_score = get_user_crypto_rating_users(users, i, t);
            if(cur_score==INFINITY)
            {                
                save_user_crypto_rating_users(users, i, t, temp_score) ;
            }
        }
        // printf("\nScore: ");
        // for(int t = 0; t < coins_num; t++)
        // {
        //     cur_score = get_user_crypto_rating_users(users, i, t);
            
        //     printf("%lf " , get_user_crypto_rating_users(users, i, t) );

        // }

    }

}

int check_if_zero(struct users* users[], long long pos, int coins_num)
{   
    int its_zero = 1;

    for(int i = 0; i < coins_num; i++)
    {
        if(((get_user_crypto_rating_users(users,pos,i)) != 0) &&  ((get_user_crypto_rating_users(users,pos,i)) != INFINITY))
        {
           its_zero = 0;
           //save_empty_user_users(users,pos,its_zero);
           return its_zero;
        }
    }

    return its_zero;
}

long long count_users_with_coins(struct users* users[], long long users_num, int coins_num)
{
    long long users_with_coins_num = 0;
    int coins_temp = 0;
    int all_zero = 1, aver_zero = 0;
    for(long long y = 0; y < users_num; y++)
    {
        if((get_tot_tweets_users(users, y))>0)
        {    
            all_zero = check_if_zero(users, y, coins_num);
            // if(!get_average_rating_users(users, y))
            // {   
            //     aver_zero = 1;
            // }
            // else
            // {
            //    aver_zero = 0;
            // }

            if(!all_zero)
            {
                users_with_coins_num++;
                save_empty_user_users(users, y, 0);
            }
            else
            {
                save_empty_user_users(users, y, 1);
                
            }
        }

        
    }
    
    return users_with_coins_num;
}

void clear_users_with_no_coins(struct users* users[], long long users_num, struct users* users_with_coins[], long long users_with_coins_num, int coins_num)
{
    long long counter = 0;
    for (long i = 0; i < users_num; i++)
    {
        if((get_empty_user_users(users,i))==0)
        {
            copy_users(users, i, users_with_coins, counter, coins_num);

            counter++;

        }
    }

}

