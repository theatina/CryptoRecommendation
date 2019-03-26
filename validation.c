//
//  validation.c
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#include "validation.h"

struct known_ratings;
struct just_a_long_long;
struct validation_set;

//validation
void validation_algo(struct tweets* tweets[], long long tweets_num, struct sentim_score_map* sentim_map[], long long sentim_map_size, struct crypto_coins* coins[], long long coins_size, struct users* users[], long long users_num, char* outputfile, int p_nearest)
{

    int dimension_of_coords = coins_size;
    
    //sentiment score
    calculate_sentim_score(tweets, tweets_num, sentim_map, sentim_map_size);

    long long known_pairs_num = 0;
    //create users
    known_pairs_num = create_users(users, users_num, tweets, tweets_num, coins, coins_size );
    printf("\nKnown pairs (all): %lld\n", known_pairs_num);
    //print_users(users, users_num,coins_size, coins);
    //normalise_vector_user_crypto(users, users_num, coins_size);
    
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
    
    known_pairs_num = count_known_pairs(users, users_num, coins_size);
    printf("\nKnown pairs (final): %lld\n", known_pairs_num);

    //create list of known pairs ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct known_ratings* known_pairs[1];
    known_pairs[0] = malloc(get_size_known_ratings()*known_pairs_num);
    if(print_msgs_recom_on==1)
        printf("\nCreating list of known ratings(user-crypto pairs)...\n");
    create_known_pairs_list(known_pairs, users, users_num, coins_size, known_pairs_num);
    //keep a copy for the validation of B
    struct known_ratings* known_pairs_copy[1];
    known_pairs_copy[0] = malloc(get_size_known_ratings()*known_pairs_num);
    copy_known_pairs_list(known_pairs, known_pairs_copy, known_pairs_num);
    // print_known_pairs(known_pairs, known_pairs_num);

    printf("\n");
    print_msg_centre_line(" Validation for A ");
    printf("\n");
    //validation for real users ( A ) --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    double MAE_LSH_A = 0.0, MAE_CLUSTERING_A = 0.0;
    struct validation_set* parts_to_validate[1];
    parts_to_validate[0] = malloc(folds * get_size_validation_set());
    if(print_msgs_recom_on==1)
        printf("\nSplitting set into subsets...\n");
    split_set(known_pairs, known_pairs_num, parts_to_validate, folds);
    //print_folds(parts_to_validate, folds);

    validate_A(&MAE_LSH_A, &MAE_CLUSTERING_A,  users, users_num, known_pairs, known_pairs_num, tweets, tweets_num, coins, coins_size, outputfile, p_nearest, parts_to_validate);
    

    //validation for virtual users ( B ) ---------------------------------------------------------------------------------------------------------------------------------------------------------
    printf("\n");
    print_msg_centre_line(" Validation for B ");
    printf("\n");

    //print_users(users, users_num, coins_size, coins);
    //create virtual users 
    if(print_msgs_recom_on == 1)
        printf("\nCreating Virtual Users by clustering(from file) the tweets...\n");
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
    //print_users(users, users_num, coins_size, coins);

    double MAE_LSH_B = 0.0, MAE_CLUSTERING_B = 0.0; 
    struct validation_set* parts_to_validate_B[1];
    parts_to_validate_B[0] = malloc(folds * get_size_validation_set());
    if(print_msgs_recom_on==1)
        printf("\nSplitting set into subsets...\n");
    split_set(known_pairs_copy, known_pairs_num, parts_to_validate_B, folds);
    //print_folds(parts_to_validate_B, folds);
    //print_users(users, users_num, coins_size, coins);
    validate_B(&MAE_LSH_B, &MAE_CLUSTERING_B,  users, users_num, users_representatives, users_representatives_num, known_pairs, known_pairs_num, tweets, tweets_num, coins, coins_size, outputfile, p_nearest, parts_to_validate_B);


    //MAE's
    printf("\n\nCosine LSH Recommendation MAE: \n\nA. %0.10lf     B. %0.10lf\n", MAE_LSH_A, MAE_LSH_B);
    printf("\n\nClustering Recommendation MAE: \n\nA. %0.10lf     B. %0.10lf\n", MAE_CLUSTERING_A, MAE_CLUSTERING_B);


    //deleteS --------------------------------------------------------------------------------
    delete_users(users_representatives, users_representatives_num, coins_size);
    delete_users(users, users_num, coins_size);
    //delete_users(users_with_coins, users_with_coins_num, coins_size);
    delete_known_ratings(known_pairs, known_pairs_num);
    delete_known_ratings(known_pairs_copy, known_pairs_num);
    delete_validation_set(parts_to_validate, folds);
    delete_validation_set(parts_to_validate_B, folds);

}

//struct and functions
struct known_ratings
{
    long long node_index;
    long long user_index;
    int coin_index;
    double real_rating;

    struct known_ratings* next;

};

struct just_a_long_long
{
    long long pair_index;
    struct just_a_long_long* next;
};

struct validation_set
{
    long long pairs_num;
    struct just_a_long_long* first_pair;
    struct just_a_long_long* last_pair;
};

size_t get_size_known_ratings(void)
{
    return (sizeof(struct known_ratings));
}

size_t get_size_just_a_long_long(void)
{
    return (sizeof(struct just_a_long_long));
}

size_t get_size_validation_set(void)
{
    return (sizeof(struct validation_set));
}


void add_pair_known_ratings(struct known_ratings* list[], long long user_index, int coin_index, double rating, long long known_index)
{
    list[0][known_index].node_index = known_index;
    list[0][known_index].user_index = user_index;
    list[0][known_index].coin_index = coin_index;
    list[0][known_index].real_rating = rating;
}

void print_known_pairs(struct known_ratings* list[], long long known_pairs_num)
{
    for(long long i = 0; i < known_pairs_num; i++ )
    {
        printf("\n\nKnown_pair%lld", i);
        printf("\nUser_index: %lld", list[0][i].user_index);
        printf("\nCoin_index: %d", list[0][i].coin_index);
        printf("\nRating: %lf", list[0][i].real_rating);
    
    }
}

void print_folds(struct validation_set* parts_to_validate[], int folds_to_make)
{
    struct just_a_long_long* templist = NULL;
    //long long pairs = 0;
    for(int i = 0; i < folds_to_make; i++)
    {   
        printf("\nSet%d:\n", i+1);
        templist = parts_to_validate[0][i].first_pair;
        //pairs = parts_to_validate[0][i].pairs_num;
        while(templist!=NULL)
        {
            printf("%lld ", templist -> pair_index);

            templist = templist -> next;
        }
       
        
    }
}


void copy_known_pairs_list(struct known_ratings* known_pairs[], struct known_ratings* known_pairs_copy[], long long known_pairs_num)
{
    for(long long i = 0; i < known_pairs_num; i++)
    {
        known_pairs_copy[0][i].node_index = i;
        known_pairs_copy[0][i].user_index = known_pairs[0][i].user_index;
        known_pairs_copy[0][i].coin_index = known_pairs[0][i].coin_index;
        known_pairs_copy[0][i].real_rating = known_pairs[0][i].real_rating;

    }

}

long long count_known_pairs(struct users* users[], long long users_num, int coins_num)
{
    long long pairs_num = 0;
    double temp_rating = 0.0;

    for(long long i = 0; i < users_num; i++)
    {
        for(int y = 0; y < coins_num; y++ )
        {
            temp_rating = get_user_crypto_rating_users(users, i, y);
            if(temp_rating!=INFINITY)
            {
                pairs_num++;
            }
        }
    }

    return pairs_num;
}

long long get_nth_pair_index_and_delete_from_known_ratings(struct known_ratings** known_list, long long n)
{
    long long index = -9, counter = 0;
    struct known_ratings* templist = NULL, *prev = NULL, *next_node = NULL;
    templist = (*known_list);
    prev = templist;

    if(templist!=NULL)
    {
        next_node = templist -> next;
    }

    while( (next_node!=NULL) && (counter < n))
    {
        prev = templist;
        templist = next_node;
        next_node = next_node -> next;
        counter++;
    }
    //printf("\nfirst_index(%lld)Pair Index: %lld (n: %lld)\n", (*known_list)->node_index, templist -> node_index, n);

    if(templist!=NULL)
    {
        index = templist -> node_index;
        if( !n/*templist==known_list*/ ) //first node of list
        {
            if(((*known_list)->next)==NULL)
            {
                (*known_list) = (*known_list) -> next;
                templist -> next = NULL;
                free(templist);
                templist = NULL;

            }
            else
            {   
                (*known_list) = (*known_list) -> next;
                templist -> next = NULL;
                //printf("\nBefore:Known_list_first: %lld\n", (*known_list)->node_index);
                free(templist);
                //printf("\nAfter:Known_list_first: %lld\n", (*known_list)->node_index);
                templist = NULL;

            }
           
        }
        else
        {
            if( (templist->next) == NULL)
            {
                
                free(templist);
                templist = NULL;
                prev -> next = NULL;
            }
            else
            {   
                prev -> next = templist -> next;
                templist -> next = NULL;
                free(templist);
                templist = NULL;

            }
        }
    }

    return index;
}

//delete
void delete_validation_set(struct validation_set* parts_to_validate[], int folds_num)
{
    struct just_a_long_long* templist = NULL, *prev = NULL;
    for(int i = 0; i < folds_num; i++)
    {
        templist = parts_to_validate[0][i].first_pair;
        while(templist!=NULL)
        {
            prev = templist;
            templist = templist -> next;

            prev -> next = NULL;
            free(prev);
            prev = NULL; 

        }

    }

    free(parts_to_validate[0]);

}

void delete_known_ratings(struct known_ratings* known_pairs[], long long known_pairs_num)
{
    free(known_pairs[0]);
}

//useful subfunctions
long long create_known_pairs_list(struct known_ratings* list[], struct users* users[], long long users_num, int coins_num, long long known_index)
{
    long long known_counter = 0;
    double temp_rating = -9;

    for (long long user_i = 0; user_i < users_num; user_i++)
    {
        for (int coin_y = 0; coin_y < coins_num; coin_y++)
        {

            temp_rating = get_user_crypto_rating_users(users, user_i, coin_y);
            if(temp_rating != INFINITY)
            {
            
                //save this pair on the list
                add_pair_known_ratings(list, user_i, coin_y, temp_rating, known_counter);
                
                known_counter++;

            }

        }
    }

    return known_counter;

}

void split_set(struct known_ratings* known_pairs[], long long known_pairs_num, struct validation_set* parts_to_validate[], int parts)
{
    //make a list of the pairs
    struct known_ratings* known_list = NULL, *templist = NULL;
    for(long long i = 0; i < known_pairs_num; i++)
    {
        struct known_ratings* newnode = NULL;
        newnode = malloc(get_size_known_ratings());
        newnode -> node_index = i;
        newnode -> user_index = known_pairs[0][i].user_index;
        newnode -> coin_index = known_pairs[0][i].coin_index;
        newnode -> real_rating = known_pairs[0][i].real_rating;
        newnode -> next = NULL;

        if (!i)
        {
            known_list = newnode;
            templist = known_list;
        }
        else
        {
            templist -> next = newnode;
            templist = templist -> next;
        }
    }
    

    long long pairs_left = known_pairs_num, pairs_to_each_part = 0, pairs_mod = 0, temp_rand = -9;
    pairs_to_each_part = known_pairs_num / parts;
    pairs_mod = known_pairs_num % parts;
    long long index_of_pair = -9;

    for(int i = 0; i < parts; i++)
    {
        if(i==parts-1)
        {
            pairs_to_each_part += pairs_mod;
        }

        for(long long y = 0; y < pairs_to_each_part; y++)
        {
            temp_rand = rand() % pairs_left; //choose a pair to put in the set
            
            //printf("\nRound %lld\n", y);
            index_of_pair = get_nth_pair_index_and_delete_from_known_ratings(&known_list, temp_rand);
            //printf("\nafter_delete_out:Known_list_first: %lld\n", known_list->node_index);

            struct just_a_long_long* newnode_longlong = NULL;
            newnode_longlong = malloc(get_size_just_a_long_long());
            newnode_longlong -> pair_index = index_of_pair;
            newnode_longlong -> next = NULL;



            if(!y)
            {
                parts_to_validate[0][i].pairs_num = 1;
                parts_to_validate[0][i].first_pair = newnode_longlong;
                parts_to_validate[0][i].last_pair = parts_to_validate[0][i].first_pair;
            }
            else
            {
                parts_to_validate[0][i].pairs_num++;
                parts_to_validate[0][i].last_pair -> next = newnode_longlong;
                parts_to_validate[0][i].last_pair = parts_to_validate[0][i].last_pair -> next;
            }

            

            pairs_left--;
        }


    }


}

void known_to_unknown(struct users* users[], long long users_num, struct known_ratings* known_pairs[], struct validation_set* parts_to_validate[], int fold)
{
    long long pair_index = -9, user_index = -9;
    int coin_index = -9;
    struct just_a_long_long* templist = NULL;
    templist = parts_to_validate[0][fold].first_pair;
    int coins_in_user = 0;

    while(templist!=NULL)
    {
        pair_index = templist -> pair_index;
        user_index = known_pairs[0][pair_index].user_index;
        coin_index = known_pairs[0][pair_index].coin_index;

        double infinity = 0.0;
        infinity = INFINITY;
        //printf("\n%d\n", __LINE__ );
        //printf("\n%lld - %d\n", user_index, coin_index);
        save_user_crypto_rating_users(users, user_index, coin_index, infinity );
        coins_in_user = get_tot_coins_users(users, user_index);
        coins_in_user--;
        save_tot_coins_users(users, user_index, coins_in_user);
        //printf("\n%d\n", __LINE__ );

        templist = templist -> next;
    }

}

void unknown_to_known(struct users* users[], long long users_num, struct known_ratings* known_pairs[], struct validation_set* parts_to_validate[], int fold)
{
    long long pair_index = -9, user_index = -9, coin_index = -9;
    struct just_a_long_long* templist = NULL;
    templist = parts_to_validate[0][fold].first_pair;
    int coins_in_user = 0;

    while(templist!=NULL)
    {
        pair_index = templist -> pair_index;
        user_index = known_pairs[0][pair_index].user_index;
        coin_index = known_pairs[0][pair_index].coin_index;

        //double original_rating = 0.0;
        //original_rating = ;
        //save_user_crypto_rating_users(users, user_index, coin_index, infinity );
        coins_in_user = get_tot_coins_users(users, user_index);
        coins_in_user++;
        save_tot_coins_users(users, user_index, coins_in_user);

        templist = templist -> next;
    }
}

//validation
double sum_fabs_dif(struct users* users[], long long users_num, int coins_size, struct known_ratings* known_pairs[], struct validation_set* parts_to_validate[], int part)
{   
    long long user_index = -9, known_index = -9;
    int coin_index = -9;
    double sum = 0.0, predicted = 0.0, actual = 0.0, dif = 0.0;
    struct just_a_long_long* templist_longlong = NULL;
    templist_longlong = parts_to_validate[0][part].first_pair;
    while(templist_longlong!=NULL)
    {
        known_index = templist_longlong -> pair_index;
        user_index = known_pairs[0][known_index].user_index;
        coin_index = known_pairs[0][known_index].coin_index;

        predicted = get_user_crypto_rating_users(users, user_index, coin_index);
        actual = known_pairs[0][known_index].real_rating;
        //printf(" %lf ", predicted);
        dif = (double)actual - (double)predicted;
        sum += fabs ((double)dif);

        templist_longlong = templist_longlong -> next;
    }

    //printf("\nsum of fabs: %lf\n", sum);

    return sum;
}

double MAE_formula(struct users* users[], long long users_num, int coins_size, struct known_ratings* known_pairs[], struct validation_set* parts_to_validate[], int part)
{   
    double mae_temp = 0.0, MAE = 0.0;
    long long valid_set_size = 0;

    valid_set_size = parts_to_validate[0][part].pairs_num;
    mae_temp += sum_fabs_dif(users, users_num, coins_size, known_pairs, parts_to_validate, part);
    MAE =  (double)((double)1/(double)valid_set_size) * (double)mae_temp;

    return MAE;
}


//lsh
double validate_lsh_A(struct users* users[], long long users_num, struct known_ratings* known_pairs[], long long known_pairs_num, struct tweets* tweets[], long long tweets_num, struct crypto_coins* coins[], long long coins_size, char* outputfile, int p_nearest, struct validation_set* parts_to_validate[], int part)
{
    double MAE = 0.0;

    //run the algorithms to calculate the unknown ratings - as in recom_steps
    calculate_average_rating(users, users_num, coins_size);
    print_line();
	print_msg_centre("1.1 Cosine LSH Recommendation (real users)");
    print_line();
    //Reccomendation_1.1 --------------------------------------------------------------------------------------------------------------------------------------    
    int dimension_of_coords = 0;
    dimension_of_coords = coins_size;
    cosine_LSH_recom_1_1( users, users_num, outputfile, dimension_of_coords, p_nearest, coins, coins_size);

    //MAE formula
    MAE = MAE_formula(users, users_num, coins_size, known_pairs, parts_to_validate, part);    

    printf("\nFold's MAE(LSH): %lf\n", MAE);

    return MAE;
}

double validate_lsh_B(struct users* users[], long long users_num, struct users* users_representatives[], long long users_representatives_num, struct known_ratings* known_pairs[], long long known_pairs_num, struct tweets* tweets[], long long tweets_num, struct crypto_coins* coins[], long long coins_size, char* outputfile, int p_nearest, struct validation_set* parts_to_validate[], int part)
{
    double MAE = 0.0;

    //run the algorithms to calculate the unknown ratings - as in recom_steps
    calculate_average_rating(users, users_num, coins_size);
    print_line();
	print_msg_centre("1.2 Cosine_LSH_Recommendation (virtual users)");
    print_line();
    //Reccomendation_1.1 --------------------------------------------------------------------------------------------------------------------------------------    
    int dimension_of_coords = 0;
    dimension_of_coords = coins_size;
    cosine_LSH_recom_1_2(users_representatives, users_representatives_num, users, users_num, outputfile, dimension_of_coords, p_nearest, coins, coins_size);

    //MAE formula
    MAE = MAE_formula(users, users_num, coins_size, known_pairs, parts_to_validate, part);    

    printf("\nFold Done\n");
    //printf("\nFold's MAE(LSH): %lf\n", MAE);

    return MAE;

}

//clustering
double validate_clustering_A(struct users* users[], long long users_num, struct known_ratings* known_pairs[], long long known_pairs_num, struct tweets* tweets[], long long tweets_num, struct crypto_coins* coins[], long long coins_size, char* outputfile, int p_nearest, struct validation_set* parts_to_validate[], int part)
{
    double MAE = 0.0;

    //run the algorithms to calculate the unknown ratings - as in recom_steps
    calculate_average_rating(users, users_num, coins_size);    
    print_line();
	print_msg_centre("2.1 Clustering_Recommendation (real users)");
    print_line();
    //Recommendation_2.1 --------------------------------------------------------------------------------------------------------------------------------------
    int dimension_of_coords = 0;
    dimension_of_coords = coins_size;
    clustering_recom_2_1(users, users_num, outputfile, dimension_of_coords, p_nearest, coins, coins_size, tweets_num);

    //MAE formula
    MAE = MAE_formula(users, users_num, coins_size, known_pairs, parts_to_validate, part);    

    printf("\nFold's MAE(CLUSTERing): %lf\n", MAE);


    return MAE;

}

double validate_clustering_B(struct users* users[], long long users_num, struct users* users_representatives[], long long users_representatives_num, struct known_ratings* known_pairs[], long long known_pairs_num, struct tweets* tweets[], long long tweets_num, struct crypto_coins* coins[], long long coins_size, char* outputfile, int p_nearest, struct validation_set* parts_to_validate[], int part)
{
    double MAE = 0.0;

    //run the algorithms to calculate the unknown ratings - as in recom_steps
    calculate_average_rating(users, users_num, coins_size);    
    print_line();
	print_msg_centre("2.2 Clustering_Recommendation (virtual users)");
    print_line();
    //Recommendation_2.1 --------------------------------------------------------------------------------------------------------------------------------------
    int dimension_of_coords = 0;
    dimension_of_coords = coins_size;
    clustering_recom_2_2(users_representatives, users_representatives_num, users, users_num, outputfile, dimension_of_coords, p_nearest, coins, coins_size, tweets_num);

    //MAE formula
    MAE = MAE_formula(users, users_num, coins_size, known_pairs, parts_to_validate, part);    

    printf("\nFold's MAE(CLUSTERing): %lf\n", MAE);


    return MAE;

}

void validate_A(double* MAE_LSH_A, double* MAE_CLUSTERING_A, struct users* users[], long long users_num, struct known_ratings* known_pairs[], long long known_pairs_num, struct tweets* tweets[], long long tweets_num, struct crypto_coins* coins[], long long coins_size, char* outputfile, int p_nearest, struct validation_set* parts_to_validate[])
{
    for (int i = 0; i < folds; i++)
    {
        print_line();
        int size_of_msg = strlen("FOLD ") + sizeof(int) + 1;
        char tempstr[size_of_msg];
        char intstr[sizeof(int)];
        sprintf(intstr, "%d", i+1);
        strcpy(tempstr, "FOLD ");
        strcat(tempstr, intstr);
        print_msg_centre(tempstr);
        print_line();
        //LSH -----------------------------------------------------------------------------
        known_to_unknown(users, users_num, known_pairs, parts_to_validate, i);

        //run the validation for LSH algorithm
        (*MAE_LSH_A) += validate_lsh_A(users, users_num, known_pairs, known_pairs_num, tweets, tweets_num, coins, coins_size, outputfile, p_nearest, parts_to_validate, i);
        //print_users(users, users_num, coins_size, coins);

        //clear before Clustering
        clear_users(users, users_num);
        retrieve_saved_user_crypto_ratings_users(users, users_num, coins_size);
        unknown_to_known(users, users_num, known_pairs, parts_to_validate, i);
        
        //Clustering -----------------------------------------------------------------------------
        //change known ratings of the validation set to unknown, for the next algorithm
        known_to_unknown(users, users_num, known_pairs, parts_to_validate, i);
        //run the validation for LSH algorithm
        (*MAE_CLUSTERING_A) += validate_clustering_A(users, users_num, known_pairs, known_pairs_num, tweets, tweets_num, coins, coins_size, outputfile, p_nearest, parts_to_validate, i);
        //print_users(users, users_num, coins_size, coins);


        //clear structs - reset for next loop
        clear_users(users, users_num); 
        retrieve_saved_user_crypto_ratings_users(users, users_num, coins_size);
        unknown_to_known(users, users_num, known_pairs, parts_to_validate, i);
    }

    // for (int i = 0; i < folds; i++)
    // {
    //     //Clustering -----------------------------------------------------------------------------
    //     //change known ratings of the validation set to unknown, for the next algorithm
    //     known_to_unknown(users, users_num, known_pairs, parts_to_validate, i);
    //     //run the validation for LSH algorithm
    //     (*MAE_CLUSTERING_A) += validate_clustering_A(users, users_num, known_pairs, known_pairs_num, tweets, tweets_num, coins, coins_size, outputfile, p_nearest, parts_to_validate, i);
    //     //print_users(users, users_num, coins_size, coins);


    //     //clear structs - reset for next loop
    //     clear_users(users, users_num); 
    //     retrieve_saved_user_crypto_ratings_users(users, users_num, coins_size);
    //     unknown_to_known(users, users_num, known_pairs, parts_to_validate, i);
    
    // }



    //formula of MAE
    (*MAE_LSH_A) = ( (double)(*MAE_LSH_A) / (double)folds );
    (*MAE_CLUSTERING_A) = ( (double)(*MAE_CLUSTERING_A) / (double)folds );

}

void validate_B(double* MAE_LSH_B, double* MAE_CLUSTERING_B, struct users* users[], long long users_num, struct users* users_representatives[], long long users_representatives_num, struct known_ratings* known_pairs[], long long known_pairs_num, struct tweets* tweets[], long long tweets_num, struct crypto_coins* coins[], long long coins_size, char* outputfile, int p_nearest, struct validation_set* parts_to_validate[])
{

    for (int i = 0; i < folds; i++)
    {
        print_line();
        int size_of_msg = strlen("FOLD ") + sizeof(int) + 1;
        char tempstr[size_of_msg];
        char intstr[sizeof(int)];
        sprintf(intstr, "%d", i+1);
        strcpy(tempstr, "FOLD ");
        strcat(tempstr, intstr);
        print_msg_centre(tempstr);
        print_line();
        //LSH -----------------------------------------------------------------------------
        //print_users(users, users_num, coins_size, coins);
        known_to_unknown(users, users_num, known_pairs, parts_to_validate, i);

        //run the validation for LSH algorithm
        (*MAE_LSH_B) += validate_lsh_B(users, users_num, users_representatives, users_representatives_num, known_pairs, known_pairs_num, tweets, tweets_num, coins, coins_size, outputfile, p_nearest, parts_to_validate, i);
        //print_users(users, users_num, coins_size, coins);

        //clear before Clustering
        clear_users(users, users_num);
        retrieve_saved_user_crypto_ratings_users(users, users_num, coins_size);
        unknown_to_known(users, users_num, known_pairs, parts_to_validate, i);
        
        //Clustering -----------------------------------------------------------------------------
        //change known ratings of the validation set to unknown, for the next algorithm
        known_to_unknown(users, users_num, known_pairs, parts_to_validate, i);
        //run the validation for LSH algorithm
        (*MAE_CLUSTERING_B) += validate_clustering_B(users, users_num, users_representatives, users_representatives_num, known_pairs, known_pairs_num, tweets, tweets_num, coins, coins_size, outputfile, p_nearest, parts_to_validate, i);
        //print_users(users, users_num, coins_size, coins);


        //clear structs - reset for next loop
        clear_users(users, users_num); 
        retrieve_saved_user_crypto_ratings_users(users, users_num, coins_size);
        unknown_to_known(users, users_num, known_pairs, parts_to_validate, i);
    }

    // for (int i = 0; i < folds; i++)
    // {
    //     //Clustering -----------------------------------------------------------------------------
    //     //change known ratings of the validation set to unknown, for the next algorithm
    //     known_to_unknown(users, users_num, known_pairs, parts_to_validate, i);
    //     //run the validation for LSH algorithm
    //     (*MAE_CLUSTERING_A) += validate_clustering_A(users, users_num, known_pairs, known_pairs_num, tweets, tweets_num, coins, coins_size, outputfile, p_nearest, parts_to_validate, i);
    //     //print_users(users, users_num, coins_size, coins);


    //     //clear structs - reset for next loop
    //     clear_users(users, users_num); 
    //     retrieve_saved_user_crypto_ratings_users(users, users_num, coins_size);
    //     unknown_to_known(users, users_num, known_pairs, parts_to_validate, i);
    
    // }



    //formula of MAE
    (*MAE_LSH_B) = ( (double)(*MAE_LSH_B) / (double)folds );
    (*MAE_CLUSTERING_B) = ( (double)(*MAE_CLUSTERING_B) / (double)folds );

}




