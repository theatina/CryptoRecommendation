//
//  recom_structs.c
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#include "recom.h"


struct coin_ref_list
{
    char* coin_ref;
    struct coin_ref_list* next;

};

struct crypto_coins
{
    int tot_num;
    struct coin_ref_list* ref_list;
    struct coin_ref_list* last_node;
    char** coin_refs[1];

};

struct sentim_score_map
{
    char* str;
    double score;

};

struct tweet_content
{
    char* word;
    struct tweet_content* next;
};

struct coins_in_tweet
{
    int coin_index;
    char* name;
    struct coins_in_tweet* next;
};

struct tweets
{
    long long tot_words;
    long long user_id;
    long long tweet_id;
    double tweet_score;
    struct tweet_content* content;
    struct tweet_content* last_word;
    int tot_coins;
    struct coins_in_tweet* coins;
    struct coins_in_tweet* coins_last_node;

};

struct list_of_tweets
{
    long long tweet;
    struct list_of_tweets* next;
};

struct users
{
    long long user_id;
    int empty_user; //if the vector is 0, it is of no use!
    double* user_crypto[1]; //vector of user-crypto currency sentiment
    double* user_crypto_saved[1];
    long long tot_tweets;
    struct list_of_tweets* first_tweet;
    struct list_of_tweets* last_tweet;
    int tot_coins;
    struct coins_in_tweet* coins;
    struct coins_in_tweet* coins_last_node;
    double average_rating;
    //5 best - real users
    struct coins_in_tweet* best_5_real[5];
    int cluster_num_real;       //cluster number - real users 2.A 
    //2 best - virtual users
    struct coins_in_tweet* best_2_represent[2];
    int cluster_num_virtual;    //cluster number - virtual 2.B
    
};

//struct functions

//coin_ref_list
//get
char* get_coinref_coin_ref_list(struct coin_ref_list* list)
{   return ( list -> coin_ref ); }

struct coin_ref_list* get_next_coin_ref_list(struct coin_ref_list* list)
{   return ( list -> next ); }

char* get_nth_coinref_coin_ref_list(struct coin_ref_list* list, long long n)
{       
    struct coin_ref_list* templist = NULL;
    long long counter = 0;
    templist = list;
    while( (templist != NULL) && (counter < n) )
    {

        counter++;
        templist = get_next_coin_ref_list(templist);
    }

    char* element = NULL;
    if(templist!=NULL)
    {
        element = get_coinref_coin_ref_list(templist);
    }

    return element;     

}

//save
void save_coinref_coin_ref_list(struct crypto_coins* coins[], long long pos, char* string)
{   
    struct coin_ref_list* newnode = NULL;
    newnode = malloc(sizeof(struct coin_ref_list));
    newnode -> coin_ref = string;
    newnode -> next = NULL;

    ( coins[0][pos].last_node ) -> next = newnode;
    coins[0][pos].last_node = newnode;

    coins[0][pos].tot_num ++;
}


//crypto_coins
//get
int get_tot_num_crypto_coins(struct crypto_coins* coins[], long long pos)
{   return ( coins[0][pos].tot_num ); }

struct coin_ref_list* get_coinreflist_crypto_coins(struct crypto_coins* coins[], long long pos)
{   return( coins[0][pos].ref_list ); }

char* get_coinref_crypto_coins(struct crypto_coins* coins[], long long pos)
{   return ( ( coins[0][pos].ref_list ) -> coin_ref ); }

struct coin_ref_list* get_lastnode_crypto_coins(struct crypto_coins* coins[], long long pos)
{   return( coins[0][pos].ref_list ); }

//save
void save_crypto_coin(struct crypto_coins* coins[], long long pos, char* string)
{   

    struct coin_ref_list* newnode = NULL;
    newnode = malloc(sizeof(struct coin_ref_list));
    newnode -> coin_ref = string;
    newnode -> next = NULL;

    coins[0][pos].ref_list = newnode;
    coins[0][pos].last_node = coins[0][pos].ref_list;

    coins[0][pos].tot_num = 1;
}

void crypto_coins_create_array(struct crypto_coins* coins[], long long coins_num)
{
    int tempnum = 0;
    for (long long u = 0; u < coins_num; u++)
    {
        tempnum = get_tot_num_crypto_coins(coins, u);
        //printf("\nCoin refs: %d", tempnum);
        coins[0][u].coin_refs[0] = malloc (tempnum * sizeof(char*));
        struct coin_ref_list* templist = get_coinreflist_crypto_coins(coins, u);
        for (int h = 0; h < tempnum; h++)
        {

            coins[0][u].coin_refs[0][h] = get_coinref_coin_ref_list(templist);

            templist = get_next_coin_ref_list(templist);
        }

    }

}

//more
int is_a_coin(char* string_to_check, struct crypto_coins* coins[], int coins_num)
{   
    int index = -1, is_same = 0, temprefs = 0;
    struct coin_ref_list* templist = NULL;
    //printf("|%s|", string_to_check);
    for (int g = 0; g < coins_num; g++)
    {   
        templist = get_coinreflist_crypto_coins(coins, g);
        temprefs = coins[0][g].tot_num;
        for(int k = 0; k < temprefs; k++)
        {
            
            is_same = check_if_same_str(coins[0][g].coin_refs[0][k], string_to_check);
            if(is_same)
            {
                //printf("Is_a_coin!! index: %d, name: %s", g, coins[0][g].coin_refs[0][k]);
                index = g;
                return index;
            }

           // templist = get_next_coin_ref_list(templist);
        }
    }

    return index;
}

char* name_of_coin_from_pos(struct crypto_coins* coins[], int coins_num, int index)
{
    char* name = NULL;
    int tot_num_of_refs = -9;
    if(index>=0)
    {
        tot_num_of_refs = coins[0][index].tot_num;
        if(tot_num_of_refs < 5 )
        {
            name = coins[0][index].coin_refs[0][0];
            
        }
        else
        {
            name = coins[0][index].coin_refs[0][4];
        }
    }

    return name;
}

//sentim_score_map
//get
char* get_srt_sentim_score_map(struct sentim_score_map* sentimap[], long long pos)
{   return ( sentimap[0][pos].str ); }

double get_score_sentim_score_map(struct sentim_score_map* sentimap[], long long pos)
{   return ( sentimap[0][pos].score ); }

double get_word_score(struct sentim_score_map* sentim_map[], long long sentim_map_size, char* tempstr)
{
    double score = 0.0;
    int same = 0;
    for(long long h = 0; h < sentim_map_size; h++)
    {
        
        same = check_if_same_str(tempstr, get_srt_sentim_score_map(sentim_map, h) );
        //printf("\n|%s| == |%s| ", tempstr,get_srt_sentim_score_map(sentim_map, h) );
        if(same)
        {
            score = get_score_sentim_score_map(sentim_map, h);
            //printf("\n-> %lf\n", score);
            //sleep(10);
        }
        

    }

    return score;
}

//save
void save_srt_sentim_score_map(struct sentim_score_map* sentimap[], long long pos, char* string)
{   sentimap[0][pos].str = string ;}

void save_score_sentim_score_map(struct sentim_score_map* sentimap[], long long pos, double score)
{   sentimap[0][pos].score = score ;}


//tweet_content
//get
char* get_word_tweet_content(struct tweet_content* list)
{   return ( list -> word ); }

struct tweet_content* get_next_tweet_content(struct tweet_content* list)
{   return ( list -> next ); }


//save
void save_word_tweet_content(struct tweets* tweets[], long long pos, char* word)
{
    struct tweet_content* newnode = NULL;
    newnode = malloc(sizeof(struct tweet_content));
    newnode -> next = NULL;
    newnode -> word = word;

    (tweets[0][pos].last_word) -> next = newnode;
    tweets[0][pos].last_word = newnode;

    tweets[0][pos].tot_words++;

}

//coins_in_tweet
//init



//tweets
//init
void tweets_init(struct tweets* tweets[], long long tweets_num)
{
    

    for(long long k = 0; k < tweets_num; k++)
    {
        tweets[0][k].tot_words = -9;
        tweets[0][k].user_id = -9;
        tweets[0][k].tweet_id = -9;
        tweets[0][k].tweet_score = -9;
        tweets[0][k].content = NULL;
        tweets[0][k].last_word = NULL;
        tweets[0][k].tot_coins = -9;
        tweets[0][k].coins = NULL;
        tweets[0][k].coins_last_node = NULL;

    }

}
//get
long long get_tot_words_tweets(struct tweets* tweets[], long long pos)
{   return (tweets[0][pos].tot_words); }

long long get_user_id_tweets(struct tweets* tweets[], long long pos)
{   return (tweets[0][pos].user_id); }

long long get_tweet_id_tweets(struct tweets* tweets[], long long pos)
{   return (tweets[0][pos].tweet_id); }

double get_tweet_score_tweets(struct tweets* tweets[], long long pos)
{  return (tweets[0][pos].tweet_score); }

struct tweet_content* get_tweet_content_tweets(struct tweets* tweets[], long long pos)
{   return (tweets[0][pos].content); }

struct tweet_content* get_last_word_tweets(struct tweets* tweets[], long long pos)
{   return (tweets[0][pos].last_word); }

long long get_pos_from_id_tweets(struct tweets* tweets[], long long tweets_num, long long tweet_id)
{
    long long pos = -1;

    for (long long o = 0; o < tweets_num; o++)
    {
        if( (tweets[0][o].tweet_id) == tweet_id)
        {
            pos = o;
            return pos;
        }
    }

    return pos;
}

//save
void save_word_tweets(struct tweets* tweets[], long long pos, char* word)
{
    struct tweet_content* newnode = NULL;
    newnode = malloc(sizeof(struct tweet_content));
    newnode -> next = NULL;
    newnode -> word = word;

    tweets[0][pos].content = newnode;
    tweets[0][pos].last_word = tweets[0][pos].content;

    tweets[0][pos].tot_words = 1;

}

void save_user_id_tweets(struct tweets* tweets[], long long pos, long long id)
{   tweets[0][pos].user_id = id; }

void save_tweet_id_tweets(struct tweets* tweets[], long long pos, long long id)
{   tweets[0][pos].tweet_id = id; }

void save_tweet_score_tweets(struct tweets* tweets[], long long pos, double score)
{  tweets[0][pos].tweet_score = score; }

//more
void init_coins_in_tweet(int coin, struct tweets* tweets[], long long pos, struct crypto_coins* coins[])
{
    struct coins_in_tweet* newnode = NULL;
    newnode = malloc(sizeof(struct coins_in_tweet));
    newnode -> coin_index = coin;
    // char* tempname = NULL;
    // tempname = get_nth_coinref_coin_ref_list(get_coinreflist_crypto_coins(coins,pos),5);
    // if(tempname==NULL)
    // {
    //     tempname = get_nth_coinref_coin_ref_list(coins[0][pos].ref_list,1);
    // }
    newnode -> name = NULL;
    newnode -> next = NULL;
    tweets[0][pos].coins = newnode;
    tweets[0][pos].coins_last_node = tweets[0][pos].coins;
    tweets[0][pos].tot_coins = 1;
}

void add_coins_in_tweet(int coin, struct tweets* tweets[], long long pos, struct crypto_coins* coins[])
{
    struct coins_in_tweet* newnode = NULL, *templist = NULL;
    int exists = 0;
    templist = tweets[0][pos].coins;
    while (templist!=NULL)
    {
        if( (templist -> coin_index) == coin )
        {

            exists = 1;
            break;
        }
        templist = templist -> next;

    }
    if(!exists)
    {
        newnode = malloc(sizeof(struct coins_in_tweet));
        newnode -> coin_index = coin;
        // char* tempname = NULL;
        // tempname = get_nth_coinref_coin_ref_list(coins[0][pos].ref_list,5);
        // if(tempname==NULL)
        // {
        //     tempname = get_nth_coinref_coin_ref_list(coins[0][pos].ref_list,1);
        // }
        // newnode -> coin_name = tempname;
        newnode -> next = NULL;
        (tweets[0][pos].coins_last_node) -> next =  newnode;
        tweets[0][pos].coins_last_node = newnode;
        tweets[0][pos].tot_coins ++;

    }

}


//get_size of structs
size_t get_size_coin_ref_list()
{   return sizeof(struct coin_ref_list); }

size_t get_size_crypto_coins()
{   return sizeof(struct crypto_coins); }

size_t get_size_sentim_score_map()
{   return sizeof(struct sentim_score_map); }

size_t get_size_tweet_content()
{   return sizeof(struct tweet_content); }

size_t get_size_tweets()
{   return sizeof(struct tweets); }

size_t get_size_list_of_tweets()
{   return sizeof(struct list_of_tweets); }

size_t get_size_users()
{   return sizeof(struct users); }

//users
void users_init(struct users* users[], long long users_num, int coins_size)
{
    for (long long k = 0; k < users_num; k++)
    {
        users[0][k].user_id = -9;
        users[0][k].empty_user = -9;
        users[0][k].user_crypto[0] = NULL;
        users[0][k].user_crypto[0] = malloc(sizeof(double)*coins_size); //the vector of users-cryptocurrency
        users[0][k].user_crypto_saved[0] = NULL;
        users[0][k].user_crypto_saved[0] = malloc(sizeof(double)*coins_size); //the vector of users-cryptocurrency
        users[0][k].first_tweet = NULL;
        users[0][k].last_tweet = NULL;
        users[0][k].tot_coins = -9;
        users[0][k].coins = NULL;
        users[0][k].coins_last_node = NULL;
        users[0][k].average_rating = INFINITY;
        for(int i = 0; i < 5; i++)
        {   
            users[0][k].best_5_real[i] = malloc(sizeof(struct coins_in_tweet));
            (users[0][k].best_5_real[i])->coin_index = -9;
            (users[0][k].best_5_real[i])->name = NULL;
            if(i < 2)
            {
                users[0][k].best_2_represent[i] = malloc(sizeof(struct coins_in_tweet));
                (users[0][k].best_2_represent[i])->coin_index = -9;
                (users[0][k].best_2_represent[i])->name = NULL;
                
            }
        }
    }
}
//get
long long get_user_id_users(struct users* users[], long long pos)
{   return (users[0][pos].user_id); }

int get_empty_user_users(struct users* users[], long long pos)
{   return users[0][pos].empty_user; }

double get_user_crypto_rating_users(struct users* users[], long long pos, int coin_pos)
{
    double rating = 0;

    rating = users[0][pos].user_crypto[0][coin_pos];

    return rating;


}

long long get_tot_tweets_users(struct users* users[], long long users_num)
{   return (users[0][users_num].tot_tweets); }

int get_tot_coins_users(struct users* users[], long long users_num)
{   return (users[0][users_num].tot_coins); }

double get_average_rating_users(struct users* users[], long long pos)
{   return (users[0][pos].average_rating); }

long long get_pos_from_id_users(struct users* users[], long long users_num, long long user_id)
{
    long long pos = -1;

    for (long long o = 0; o < users_num; o++)
    {
        if( (users[0][o].user_id) == user_id)
        {
            pos = o;
            return pos;
        }
    }

    return pos;
}

struct coins_in_tweet* get_nth_best_5_real_users(struct users* users[], long long pos, int n)
{   return (users[0][pos].best_5_real[n] ); }

int get_cluster_num_real_users(struct users* users[], long long pos)
{   return (users[0][pos].cluster_num_real); }

struct coins_in_tweet* get_nth_best_2_represent_users(struct users* users[], long long pos, int n)
{   return (users[0][pos].best_2_represent[n] ); }

int get_cluster_num_virtual_users(struct users* users[], long long pos)
{   return (users[0][pos].cluster_num_virtual); }

//save
void save_empty_user_users(struct users* users[], long long pos, int is_empty)
{   users[0][pos].empty_user = is_empty; }

void save_average_rating_users(struct users* users[], long long pos, double rating)
{   users[0][pos].average_rating = rating; }

void save_user_crypto_rating_users(struct users* users[], long long pos, int coin_pos, double new_rating)
{   //printf("\n%d\n" , __LINE__);
    users[0][pos].user_crypto[0][coin_pos] = new_rating; 
    //printf("\n%d\n" , __LINE__); 
}

void save_nth_best_5_real_users(struct users* users[], long long pos, int n, int coin_index, char* name)
{   
    users[0][pos].best_5_real[n]->coin_index = coin_index;
    users[0][pos].best_5_real[n]->name = name;
}

void save_cluster_num_real_users(struct users* users[], long long pos, int cluster_num_real)
{   users[0][pos].cluster_num_real = cluster_num_real; }

void save_nth_best_2_represent_users(struct users* users[], long long pos, int n, int coin_index, char* name)
{   
    users[0][pos].best_2_represent[n]->coin_index = coin_index;
    users[0][pos].best_2_represent[n]->name = name;
}

void save_cluster_num_virtual_users(struct users* users[], long long pos, int cluster_num_virtual)
{   users[0][pos].cluster_num_virtual = cluster_num_virtual; }

void save_tot_coins_users(struct users* users[], long long pos, int coins_in_user)
{   users[0][pos].tot_coins = coins_in_user ; }


//copy
void copy_users(struct users* users[], long long pos1, struct users* users_with_coins[], long long pos2, int coins_num)
{

    //check  - - - -
    // //5 best - real users
    // struct coins_in_tweet* best_5_real[5];
    // int cluster_num_real;       //cluster number - real users 2.A 
    // //2 best - virtual users
    // struct coins_in_tweet* best_2_represent[2];
    // int cluster_num_virtual;    //cluster number - virtual 2.B


    users_with_coins[0][pos2].user_id = users[0][pos1].user_id;
    users_with_coins[0][pos2].empty_user = users[0][pos1].empty_user;
    //users_with_coins[0][pos2].user_crypto[0] = users[0][pos1].user_crypto[0];
    for(int i = 0; i < coins_num; i++)
    {   
        users_with_coins[0][pos2].user_crypto[0][i] = users[0][pos1].user_crypto[0][i];

    }
    //users_with_coins[0][pos2].user_crypto_saved[0] = users[0][pos1].user_crypto_saved[0];
    // for(int i = 0; i < coins_num; i++)
    // {   
    //     users_with_coins[0][pos2].user_crypto_saved[0][i] = users[0][pos1].user_crypto_saved[0][i];

    // }
    users_with_coins[0][pos2].tot_tweets = users[0][pos1].tot_tweets;
    users_with_coins[0][pos2].first_tweet = users[0][pos1].first_tweet;
    users_with_coins[0][pos2].last_tweet = users[0][pos1].last_tweet;
    users_with_coins[0][pos2].tot_coins = users[0][pos1].tot_coins;
    users_with_coins[0][pos2].coins = users[0][pos1].coins;
    users_with_coins[0][pos2].coins_last_node = users[0][pos1].coins_last_node;
    users_with_coins[0][pos2].average_rating = users[0][pos1].average_rating;

}

void user_crypto_save_users(struct users* users[], long long users_num, int dimension_of_coords)
{
    for (long long i = 0; i < users_num; i++)
    {
        for( int y = 0; y < dimension_of_coords; y++)
        {
            users[0][i].user_crypto_saved[0][y] = users[0][i].user_crypto[0][y] ;
        }

    }
}

void retrieve_saved_user_crypto_ratings_users(struct users* users[], long long users_num, int coins_size)
{

    for (long long y = 0; y < users_num; y++)
    {
        for (int i = 0; i < coins_size; i++)
        {
            //printf("\n %lf \n", users[0][y].user_crypto_saved[0][i]);
            users[0][y].user_crypto[0][i] = users[0][y].user_crypto_saved[0][i] ;
        }
    }

}

// void restart_users(struct users* users[], long long users_num, int coins_size)
// {

//     retrieve_saved_user_crypto_ratings_users(users, users_num, coins_size);


// }


void clear_users(struct users* users[], long long users_num)
{

    for ( long long i = 0; i < users_num; i++)
    {
        for(int y = 0; y < 5; y++)
        {
            if(y < 2)
            {
                users[0][i].best_2_represent[y]->coin_index = -9;
                (users[0][i].best_2_represent[y])->name = NULL;
            }

            users[0][i].best_5_real[y]->coin_index = -9; 
            (users[0][i].best_5_real[y])->name = NULL;
        }
        
    }





}



//create
int add_tweet_to_user(struct users* users[], long long pos,  long long tweet_index)
{
    if( (users[0][pos].first_tweet) == NULL ) //init
    {
        struct list_of_tweets* newnode = NULL;
        newnode = malloc(sizeof(struct list_of_tweets));
        newnode -> tweet = tweet_index;
        newnode -> next = NULL;
        users[0][pos].first_tweet = newnode;
        users[0][pos].last_tweet = users[0][pos].first_tweet;
        users[0][pos].tot_tweets = 1;
    }
    else //else add at the end
    {   
        struct list_of_tweets* newnode = NULL;
        newnode = malloc(sizeof(struct list_of_tweets));
        newnode -> tweet = tweet_index;
        newnode -> next = NULL;
        (users[0][pos].last_tweet) -> next = newnode;
        users[0][pos].last_tweet = newnode;
        (users[0][pos].tot_tweets) ++;

    }

    return 6;
}

int user_coin_score(struct users* users[], long long users_counter, struct tweets* tweets[], long long u, struct coins_in_tweet* templist)
{
    int known_rating = 0;
    double temp = users[0][users_counter].user_crypto[0][templist -> coin_index];
    //printf("\nUser: %lld - Tweet: %lld - score: %lf + %lf = ", users_counter, u, temp, tweets[0][u].tweet_score);

    if(temp == INFINITY)
    {
        known_rating = 1;
        users[0][users_counter].user_crypto[0][templist -> coin_index] = tweets[0][u].tweet_score;
    }
    else
    {
        (users[0][users_counter].user_crypto[0][templist -> coin_index]) += tweets[0][u].tweet_score;
    }
    //printf("%lf", users[0][users_counter].user_crypto[0][templist -> coin_index]);



    return known_rating;
}

long long create_users(struct users* users[], long long users_num, struct tweets* tweets[], long long tweets_num, struct crypto_coins* coins[], int coins_size )
{
    long long known_ratings = 0;
    long long temp_user_id = -9;
    long long users_counter = 0;
    //long long u = 0;
    for (long long h = 0; h < users_num; h++)
    {
        users[0][h].tot_coins = 0;
        users[0][h].tot_tweets = 0;


    }

    for(long long u = 0; u < tweets_num; u++)
    {
        
        if(temp_user_id < 0)
        {
            temp_user_id = tweets[0][u].user_id ;
            users[0][users_counter].user_id = temp_user_id;
            //users[0][users_counter].user_crypto[0] = malloc(sizeof(double)*coins_size); //the vector of users-cryptocurrency
            for(long long k = 0; k < coins_size; k++)
            {
                users[0][users_counter].user_crypto[0][k] = INFINITY;

            }
            // for(long long k = 0; k < coins_size; k++)
            // {
            //     printf("\n%lf", users[0][users_counter].user_crypto[0][k] );
            // }
        }
        else
        {
            if(temp_user_id!=tweets[0][u].user_id)
            {
                users_counter++;
                temp_user_id = tweets[0][u].user_id;
                users[0][users_counter].user_id = temp_user_id;
                //users[0][users_counter].user_crypto[0] = malloc(sizeof(double)*coins_size); //the vector of users-cryptocurrency
                for(long long k = 0; k < coins_size; k++)
                {
                    users[0][users_counter].user_crypto[0][k] = INFINITY;
                }
                
            }
        }

        add_tweet_to_user(users, users_counter, u);



        struct coins_in_tweet* templist = NULL;
        templist = tweets[0][u].coins;
        while(templist!=NULL)
        {
           
            //printf("\nuser%lld - score: %lf", users[0][users_counter].user_id, tweets[0][u].tweet_score);
            if(users[0][users_counter].tot_coins == 0)
            {
                init_coins_in_user(templist->coin_index, users, users_counter, coins);
            }
            else
            {
                add_coins_in_user(templist->coin_index, users, users_counter, coins);

            }

            known_ratings += user_coin_score(users, users_counter, tweets, u, templist);
            //printf("%d", __LINE__);

            templist = templist->next;
        }

        //u++;
       

    }



    return known_ratings;
}


long long create_represent(struct cluster* clusters[], int clusters_num, struct users* users_representatives[], struct tweets* tweets[], long long tweets_num, struct crypto_coins* coins[], int coins_size )
{
    //long long temp_user_id = -9;
    //long long users_counter = clusters_num;
    //long long u = 0;
    long long tweet_index = -9, tweet_id = -9;

    for (long long h = 0; h < clusters_num; h++)
    {
        users_representatives[0][h].tot_coins = 0;
        users_representatives[0][h].tot_tweets = 0;
    }

    for(long long u = 0; u < clusters_num; u++)
    {
        
        users_representatives[0][u].user_id = u;
        // users_representatives[0][u].user_crypto[0] = malloc(sizeof(double)*coins_size); //the vector of users-cryptocurrency
        for(long long k = 0; k < coins_size; k++)
        {
            users_representatives[0][u].user_crypto[0][k] = INFINITY;
        }
        // for(long long k = 0; k < coins_size; k++)
        // {
        //     printf("\n%lf", users[0][users_counter].user_crypto[0][k] );
        // }

        struct points_in_cluster* templist_points_in_cluster = NULL;
        templist_points_in_cluster = get_firstpoint_in_cluster(u, clusters);
        struct coins_in_tweet* templist = NULL;
        while(templist_points_in_cluster!=NULL)
        {   
            //printf("\nPoints in cluster!");
            tweet_id = atoi(get_name_multipoints(get_point_points_in_cluster(templist_points_in_cluster)));
            tweet_index = get_pos_from_id_tweets(tweets, tweets_num, tweet_id);
            add_tweet_to_user(users_representatives, u, tweet_index);

            //score
            templist = tweets[0][tweet_index].coins;
            while(templist!=NULL)
            {
                //user_coin_score(users_representatives, u, tweets, tweet_index, templist);

                if(users_representatives[0][u].tot_coins == 0)
                {
                    init_coins_in_user(templist->coin_index, users_representatives, u, coins);
                }
                else
                {
                    add_coins_in_user(templist->coin_index, users_representatives, u, coins);

                }

                user_coin_score(users_representatives, u, tweets, tweet_index, templist);

                //printf("%d", __LINE__);

                templist = templist->next;
            }

            templist_points_in_cluster = get_next_points_in_cluster(templist_points_in_cluster);
        }


        struct list_of_tweets* templl = NULL;
        templl =  users_representatives[0][u].first_tweet;
        //printf("\nUser's tweets: ");
        // while(templl!=NULL)
        // {   
        //     printf("%lld (%lf) ", templl->tweet, tweets[0][templl->tweet].tweet_score);

        //     templl = templl -> next;
        // }
        // printf("\n");

        // struct coins_in_tweet* templist = NULL;
        // templist = tweets[0][tweet_index].coins;
        // while(templist!=NULL)
        // {
        //     //user_coin_score(users_representatives, u, tweets, tweet_index, templist);

        //     if(users_representatives[0][u].tot_coins == 0)
        //     {
        //         init_coins_in_user(templist->coin_index, users_representatives, u, coins);
        //     }
        //     else
        //     {
        //         add_coins_in_user(templist->coin_index, users_representatives, u, coins);

        //     }

        //     user_coin_score(users_representatives, u, tweets, tweet_index, templist);

        //     //printf("%d", __LINE__);

        //     templist = templist->next;
        // }

        //u++;
       

    }




    return 77;
}

//coins_users
void init_coins_in_user(int coin, struct users* users[], long long pos, struct crypto_coins* coins[])
{
    struct coins_in_tweet* newnode = NULL;
    newnode = malloc(sizeof(struct coins_in_tweet));
    newnode -> coin_index = coin;
    // char* tempname = NULL;
    // tempname = get_nth_coinref_coin_ref_list(get_coinreflist_crypto_coins(coins,pos),5);
    // if(tempname==NULL)
    // {
    //     tempname = get_nth_coinref_coin_ref_list(coins[0][pos].ref_list,1);
    // }
    // newnode -> coin_name = tempname;
    newnode -> next = NULL;
    users[0][pos].coins = newnode;
    users[0][pos].coins_last_node = users[0][pos].coins;
    users[0][pos].tot_coins = 1;
    //printf("\nCoin %d inserted to user: %lld", coin, users[0][pos].user_id);

}

void add_coins_in_user(int coin, struct users* users[], long long pos, struct crypto_coins* coins[])
{
    struct coins_in_tweet* newnode = NULL, *templist = NULL;
    int exists = 0;
    templist = users[0][pos].coins;
    while (templist!=NULL)
    {
        if( (templist -> coin_index) == coin )
        {

            exists = 1;
            break;
        }
        templist = templist -> next;

    }
    if(!exists)
    {
        newnode = malloc(sizeof(struct coins_in_tweet));
        newnode -> coin_index = coin;
        // char* tempname = NULL;
        // tempname = get_nth_coinref_coin_ref_list(coins[0][pos].ref_list,5);
        // if(tempname==NULL)
        // {
        //     tempname = get_nth_coinref_coin_ref_list(coins[0][pos].ref_list,1);
        // }
        // newnode -> coin_name = tempname;
        newnode -> next = NULL;
        (users[0][pos].coins_last_node) -> next =  newnode;
        users[0][pos].coins_last_node = newnode;
        (users[0][pos].tot_coins)++;
        //printf("\nCoin %d inserted to user: %lld(tot_coins:%d)", coin, users[0][pos].user_id, users[0][pos].tot_coins);

    }

}

int check_if_known_coin(struct users* users[], long long pos, int coin_pos)
{
    int known = 0;
    struct coins_in_tweet* templist = NULL;
    templist = users[0][pos].coins;
    while(templist!=NULL)
    {
        if((templist -> coin_index) == coin_pos)
        {
            known = 1;
            //printf("\nKNOWN - %d", templist -> coin_index);
            break;
        }

        templist = templist -> next;
    }


    return known;
}

//save data in structures
int save_data_sentim_score_map(struct sentim_score_map* structure[], const char* filename)
{
    FILE* fd = NULL;
    if ((fd = fopen(filename, "r")) == NULL)
  	{ 	perror("\nfopen source-file"); printf("\n"); return -89; }
    
    size_t len = 0;
    ssize_t linelen = 0;
    char delimit[]="\n\t";
    
    char *token = NULL;
    long long i = 0;

    while(!feof(fd))
    {
        double score = 0.0;
        char *temp = NULL;
        char *ttemp = NULL;
        
        linelen = getline(&temp, &len, fd);  
        ttemp = temp;

        if ( strlen(temp)<1 || temp == NULL )
        {
            printf("\nWarning: Empty line detected!\n");
        }    
        else
        {

            if( (temp[linelen-2] == '\n') || (temp[linelen-1] == '\n') )
            {
                *(temp+linelen-2) = '\0';
            }
            
            long long y = 0;
            while (temp !=NULL && (token = strtok_r(temp, delimit, &temp))!=NULL)
            {
                
                if (y == 0)
                {
                    char *string = NULL;
                    int stringsize = strlen(token)+1;
                    string = malloc(stringsize*sizeof(char));
                    memset((void*)string, 0, stringsize*sizeof(char));
                    
                    strncpy(string, token, stringsize-1);
                    *(string+stringsize-1) = '\0';

                    //printf("\n%lld. %s",i, string);

                    save_srt_sentim_score_map(structure, i, string);
                   
                }
                else if (y == 1)
                {       
                    //printf("| %s", token);
                    score = atof(token);
                    //printf(" | %.1lf", score);

                    save_score_sentim_score_map(structure, i, score);

                }

                y++;
                
            }

        }
        
        i++;
        free(ttemp);
        ttemp = NULL;
        //free(temp);
        temp = NULL;
        len = 0;
    }

    fclose(fd);

    return 7;
    

}

int save_data_crypto_coins(struct crypto_coins* structure[], const char* filename)
{
    FILE* fd = NULL;
    if ((fd = fopen(filename, "r")) == NULL)
  	{ 	perror("\nfopen source-file"); printf("\n"); return -89; }
    
    size_t len = 0;
    ssize_t linelen = 0;
    char delimit[]="\n\t";
    
    char *token = NULL;
    long long i = 0;

    while(!feof(fd))
    {
        //double score = 0;
        char *temp = NULL;
        char *ttemp = NULL;
        
        linelen = getline(&temp, &len, fd);  
        ttemp = temp;

        if ( strlen(temp)<1 || temp == NULL )
        {
            printf("\nWarning: Empty line detected!\n");
        }    
        else
        {
            if( (temp[linelen-2] == '\n') || (temp[linelen-1] == '\n') )
            {
                *(temp+linelen-2) = '\0';
            }
            
            long long y = 0;
            while (temp !=NULL && (token = strtok_r(temp, delimit, &temp))!=NULL)
            {
                
                if (y == 0)
                {
                    char *string = NULL;
                    int stringsize = strlen(token)+1;
                    string = malloc(stringsize*sizeof(char));
                    memset((void*)string, 0, stringsize*sizeof(char));

                    
                    strncpy(string, token, stringsize-1);
                    *(string+stringsize-1) = '\0';

                    save_crypto_coin(structure, i, string);
                   
                }
                else if (y > 0)
                {       

                    char *string = NULL;
                    int stringsize = strlen(token)+1;
                    string = malloc(stringsize*sizeof(char));
                    memset((void*)string, 0, stringsize*sizeof(char));

                    
                    strncpy(string, token, stringsize-1);
                    *(string+stringsize-1) = '\0';

                    save_coinref_coin_ref_list(structure, i, string);

                    
                }

                y++;
                
            }

        }
        
        i++;
        free(ttemp);
        ttemp = NULL;
        //free(temp);
        temp = NULL;
        len = 0;
    }

    fclose(fd);

    return 6;
}

long long save_data_tweets(struct tweets* structure[], const char* filename, struct crypto_coins* coins[], int coins_num, int* p_nearest)
{
    FILE* fd = NULL;
    if ((fd = fopen(filename, "r")) == NULL)
  	{ 	perror("\nfopen source-file"); printf("\n"); return -89; }

    long long users_num = 0;
    int same = 0;
    long long temp_user_id = -9;

    size_t len = 0;
    ssize_t linelen = 0;
    char delimit[]=" \n\t";
    
    char *token = NULL;
    long long i = 0, pos = 0;
    long long t_id = 0, u_id = 0;
    int coin_index = -1;
    char* tempstr = NULL;
    //long long temp_user_id = -9;
    //long long users_counter = 0;

    while(!feof(fd))
    {
        // fgets(tempstr, BUFSIZE, fd);
        // printf("|%s|", tempstr)
        //double score = 0;
        char *temp = NULL;
        char *ttemp = NULL;
        
        linelen = getline(&temp, &len, fd);  
        ttemp = temp;

        if ( strlen(temp)<1 || temp == NULL )
        {
            printf("\nWarning: Empty line detected!\n");
        }    
        else
        {
            if(!i)
            {
                //read P neighbours
                strtok_r(temp, delimit, &temp);
                token = strtok_r(temp, delimit, &temp);
                (*p_nearest) = atoi(token);
                printf("\n\nP_nearest: %d\n", *p_nearest);
                //printf("|%d|\n", *p_nearest);
                //sleep(7);

            }
            else
            {

                
                // char* new_line = NULL;
                // new_line = strstr(temp,"\n");
                //if(temp[linelen-2] == '\n')
                //{
                    //*(temp+linelen-2) = '\0';
                    
                //}
                if( (temp[linelen-2] == '\n') || (temp[linelen-1] == '\n') )
                {
                    *(temp+linelen-2) = '\0';
                }

                //printf(">>%s", temp);
                long long y = 0;
                while (temp !=NULL && (token = strtok_r(temp, delimit, &temp))!=NULL)
                {
                    
                    if (y == 0)
                    {
                        u_id = atoi(token);
                        //printf(" | %.1lf", score);
                        //
                        // if(temp_user_id < 0)
                        // {
                        //     temp_user_id = u_id ;
                        //     users[0][pos].user_id = temp_user_id;
                        //     users[0][pos].user_crypto[0] = malloc(sizeof(double)*coins_size); //the vector of users-cryptocurrency
                        
                        // }
                        // else
                        // {
                        //     if(temp_user_id!=u_id)
                        //     {
                        //         users_counter++;
                        //         temp_user_id = u_id;
                        //         users[0][pos].user_id = temp_user_id;
                        //         users[0][pos].user_crypto[0] = malloc(sizeof(double)*coins_size); //the vector of users-cryptocurrency
                        
                                
                        //     }
                        // }
                        // save_user_id_tweets(structure, pos, id);

                        // //count users
                        // if(temp_user_id == -9)
                        // {
                        //     temp_user_id = id;
                        //     users_num++;
                        // }
                        // else
                        // {
                        //     if(temp_user_id!=id)
                        //     {
                        //         temp_user_id = id;
                        //         users_num++;
                        //     }

                        // }
                    
                    }
                    else if( y == 1 )
                    {
                        
                        t_id = atoi(token);
                        structure[0][pos].coins = NULL;
                        structure[0][pos].coins_last_node = NULL;
                        //printf(" | %.1lf", score);
                        save_tweet_id_tweets(structure, pos, t_id);

                        //save the user_id
                        save_user_id_tweets(structure, pos, u_id);
                        //users
                        //add_tweet_to_user(users, u_id, t_id);

                        //count users
                        if(temp_user_id == -9)
                        {
                            temp_user_id = u_id;
                            users_num++;
                        }
                        else
                        {
                            if(temp_user_id!=u_id)
                            {
                                temp_user_id = u_id;
                                users_num++;
                            }

                        }
                    }
                    else if( y == 2 )
                    {
                        char *string = NULL;
                        int stringsize = strlen(token)+1;
                        string = malloc(stringsize*sizeof(char));
                        memset((void*)string, 0, stringsize*sizeof(char));

                        strncpy(string, token, stringsize-1);
                        *(string+stringsize-1) = '\0';
                        //printf("\n|%s|",string, strlen(string));

                        save_word_tweets(structure, pos, string);
                        coin_index = is_a_coin(string, coins, coins_num);
                        
                        if(coin_index >= 0)
                        {
                            if(structure[0][pos].coins == NULL)
                            {
                                init_coins_in_tweet(coin_index, structure, pos, coins);
                            }
                            else
                            {
                                add_coins_in_tweet(coin_index, structure, pos, coins);

                            }
                            
                            coin_index = -1;
                        }
                        

                    }
                    else if (y > 2)
                    {       

                        char *string = NULL;
                        int stringsize = strlen(token)+1;
                        string = malloc(stringsize*sizeof(char));
                        memset((void*)string, 0, stringsize*sizeof(char));

                        
                        strncpy(string, token, stringsize-1);
                        *(string+stringsize-1) = '\0';
                        //printf("\n|%s|",string, strlen(string));

                        save_word_tweet_content(structure, pos, string);
                        coin_index = is_a_coin(string, coins, coins_num);
                        if(coin_index >= 0)
                        {
                            if(structure[0][pos].coins == NULL)
                            {
                                init_coins_in_tweet(coin_index, structure, pos, coins);
                            }
                            else
                            {
                                add_coins_in_tweet(coin_index, structure, pos, coins);

                            }
                            
                            coin_index = -1;
                        }
                        
                        
                    }

                    y++;
                    
                }
                pos++;
            }

            i++;
        }
        
        
        free(ttemp);
        ttemp = NULL;
        //free(temp);
        temp = NULL;
        len = 0;
    }

    fclose(fd);

    return users_num;
}



//print
void print_sentim_score_map(struct sentim_score_map* sentimscore_map[], long long size)
{
    for(long long y = 0; y < size; y++)
    {
        printf("\n%lld. ", y);
        printf("%s | ", get_srt_sentim_score_map(sentimscore_map, y));
        printf("%.1lf", get_score_sentim_score_map(sentimscore_map, y));

    }

}

void print_coin_ref_list(struct coin_ref_list* list)
{
    struct coin_ref_list* templist = NULL;
    templist = list;
    while (templist != NULL)
    {
        printf("%s ", get_coinref_coin_ref_list(templist));

        templist = get_next_coin_ref_list(templist);
    }
}

void print_crypto_coins(struct crypto_coins* coins[], long long size)
{
    for(long long i = 0; i < size; i++)
    {
        printf("\n ");
        //fflush(stdout);
        printf("%lld. ", i);
        printf("(%d) ", get_tot_num_crypto_coins(coins, i));
        print_coin_ref_list( coins[0][i].ref_list);

        
    }

}

void print_crypto_coins_array(struct crypto_coins* coins[], long long size)
{   
    int tempnum = 0;
    for(long long i = 0; i < size; i++)
    {
        printf("\n ");
        //fflush(stdout);
        printf("%lld. ", i);
        tempnum = get_tot_num_crypto_coins(coins, i);
        printf("(%d) ", tempnum);
        for (int p = 0; p < tempnum; p++)
        {
            printf("|%s| ", coins[0][i].coin_refs[0][p]);
        }

        
    }

}

void print_tweet_content(struct tweets* tweets[], long long pos)
{
    struct tweet_content* templist = NULL;
    templist = tweets[0][pos].content;
    while(templist!=NULL)
    {   int temp = strlen(get_word_tweet_content(templist));
        printf("|%s| ", get_word_tweet_content(templist));
        templist = get_next_tweet_content(templist);
    }
}

void print_coins_in_tweet(struct tweets* tweets[], long long i, struct crypto_coins* coins[])
{
   // printf("In coins_in_tweet");
    char* tempname = NULL;
    int index = -1;
    struct coins_in_tweet* templist = NULL;
    int if_coin = 0;

    // templist = tweets[0][i].coins;
    printf("\n Coins_in_tweet: ");
    if (tweets[0][i].coins == NULL)
    {    
        printf("none");      
    }
    else
    {
        templist = tweets[0][i].coins;
    }
    while(templist!=NULL)
    {

        //printf("%d",templist->coin_index);
        // if(templist->coin_index >= 0)
        // {
        //     if_coin = 1;
        // }
        // if(if_coin)
        // {  
            index = (templist -> coin_index);
            tempname = get_nth_coinref_coin_ref_list(coins[0][index].ref_list, 5);
            //printf("\nTempname:\n");
            //printf(" %s", tempname);
            if(tempname == NULL)
            {
                //tempname = coins[0][index].coin_refs[0][0];
                tempname = get_nth_coinref_coin_ref_list(coins[0][index].ref_list, 1);
                printf(" %s", coins[0][index].coin_refs[0][0]);
            }
            else
            {
                printf(" %s", coins[0][index].coin_refs[0][4]);
            }
            //if(tempname!=NULL)
            //{
                //fflush(stdout);
                //printf("%s ", tempname);
                printf("(%d) ", templist->coin_index);
            //}
            
        // }
        templist = templist -> next;
        if_coin = 0;
    }
}

void print_tweets(struct tweets* tweets[], long long size, struct crypto_coins* coins[])
{
    printf("\nPrinting..");
    printf("\niwashere");
    long long i = 0;
    for (i = 0; i < size; i++)
    {
        printf("\n %lld. ", i);
        printf(" (score: %.6lf) %lld %lld (%lld) ", get_tweet_score_tweets(tweets, i), get_user_id_tweets(tweets, i), get_tweet_id_tweets(tweets, i), get_tot_words_tweets(tweets, i));
        print_tweet_content(tweets, i);
        print_coins_in_tweet(tweets, i, coins);
        
    }

}


void print_coins_in_user(struct users* users[], long long i, struct crypto_coins* coins[], int coins_num)
{
   // printf("In coins_in_tweet");
    char* tempname = NULL;
    int index = -1;
    struct coins_in_tweet* templist = NULL;
    int if_coin = 0;

    // templist = tweets[0][i].coins;
    printf("\n Coins_in_user: ");
    if (users[0][i].coins == NULL)
    {    
        printf("none");      
    }
    else
    {
        templist = users[0][i].coins;
    }
    while(templist!=NULL)
    {

        //printf("%d",templist->coin_index);
        // if(templist->coin_index >= 0)
        // {
        //     if_coin = 1;
        // }
        // if(if_coin)
        // {  
            index = (templist -> coin_index);
            tempname = name_of_coin_from_pos(coins, coins_num, index);
            //tempname = get_nth_coinref_coin_ref_list(coins[0][index].ref_list, 5);
            //printf("\nTempname:\n");
            //printf("\n%s ", tempname);
            printf(" |%s|(%d)", name_of_coin_from_pos(coins, coins_num, index), templist->coin_index);
            //printf("(%d) ", templist->coin_index);
            // if(tempname == NULL)
            // {
            //     //tempname = coins[0][index].coin_refs[0][0];
            //     tempname = get_nth_coinref_coin_ref_list(coins[0][index].ref_list, 1);
               
            // }
            // else
            // {
            //     printf(" %s", coins[0][index].coin_refs[0][4]);
            // }
            //if(tempname!=NULL)
            //{
                //fflush(stdout);
                //printf("%s ", tempname);
                
            //}
            
        // }
        templist = templist -> next;
        //if_coin = 0;
    }
}

void print_users(struct users* users[], long long users_num, int coins_num, struct crypto_coins* coins[])
{
    for(long long j = 0; j <  users_num; j++)
    {
        printf("\nUser: %lld\nTotweets: %lld\nTot_coins: %d\nScore: ", users[0][j].user_id, users[0][j].tot_tweets , users[0][j].tot_coins);
        for(int f = 0; f < coins_num; f++)
        {   
           printf("%lf ", users[0][j].user_crypto[0][f]);
        }
        printf("\nAverage_rating: %lf", users[0][j].average_rating );
        print_coins_in_user(users, j, coins, coins_num);
        

    }   
}


void print_best5_nth_user_users(struct users* users[], long long pos)
{
    printf("\n<u%lld> ", users[0][pos].user_id);
    int proposals = 0;
    for(int i = 0; i < 5; i++)
    {
        if((users[0][pos].best_5_real[i]->coin_index)>=0)
        {
            proposals = 1;
            printf("    %s", users[0][pos].best_5_real[i]->name);
        }
    }

    if(!proposals)
    {
        printf("none ");
    }
}

void print_best2_nth_user_users(struct users* users[], long long pos)
{
    printf("\n<u%lld> ", users[0][pos].user_id);
    int proposals = 0;
    for(int i = 0; i < 2; i++)
    {
        if((users[0][pos].best_2_represent[i]->coin_index)>=0)
        {
            proposals = 1;
            printf("    %s", users[0][pos].best_2_represent[i]->name);
        }
    }

    if(!proposals)
    {
        printf("none ");
    }
}

void write_to_outputfile_recom(char* outputfile, struct users* users[], long long users_num,  double recom_time, int problem)
{
    FILE* fd = NULL;    

    if(problem == 1) //cosine-lsh - real users
    {
        //open file to write - overwrite
        fd = fopen(outputfile, "w+");
        fprintf(fd, "> Cosine LSH Recommendation" );
    }
    else if(problem == 2) //clustering - real userss
    {
        //open file to write - append
        //fd = fopen(outputfile, "a");
        
        //create separate file for clustering recommendation
        char *filename = NULL;
        int filenamesize = strlen("_clustering.txt") + strlen(outputfile) + 1;
        filename = malloc(sizeof(char) * filenamesize);
        memset((void*)filename, 0, filenamesize*sizeof(char));

        strcpy(filename, outputfile);
        strcat(filename, "_clustering.txt");

        fd = fopen(filename, "w+");
        fprintf(fd, "> Clustering Recommendation" );
    }
    // else if(problem == 22) //cosine-lsh - representative users
    // {
    //     //open file to write - overwrite
    //     fd = fopen(outputfile, "ab+");
    //     fprintf(fd, "Clustering - Representative Users" );
    // }

    for (long long pos = 0; pos < users_num; pos++)
    {
        //with real users
        fprintf(fd, "\n<u%lld> ", users[0][pos].user_id);
        int proposals = 0;
        for(int i = 0; i < 5; i++)
        {
            if((users[0][pos].best_5_real[i]->coin_index)>=0)
            {
                proposals = 1;
                fprintf(fd, "   %s", users[0][pos].best_5_real[i]->name);
            }
        }

        if(!proposals)
        {
            fprintf(fd, "   none");
        }

        //with virtual users
        proposals = 0;
        fprintf(fd, "    ---||---");
        for(int i = 0; i < 2; i++)
        {
            if((users[0][pos].best_2_represent[i]->coin_index)>=0   )
            {
                proposals = 1;
                fprintf(fd, "   %s", users[0][pos].best_2_represent[i]->name);
            }
        }

        if(!proposals)
        {
            fprintf(fd, "   none");
        }


        
        
    }

    fprintf(fd, "\nExecution Time: <%.10lf>\n\n", recom_time);


}

void print_all_results(struct users* users[], long long users_num,  double recom_time, int problem)
{
    if(problem == 1) //cosine-lsh - real users
    {
        printf("\n> Cosine LSH Recommendation" );
    }
    else if(problem == 2) //clustering - real userss
    {
        printf("\n> Clustering Recommendation" );
    }

    for (long long pos = 0; pos < users_num; pos++)
    {
        //with real users
        printf("\n<u%lld> ", users[0][pos].user_id);
        int proposals = 0;
        for(int i = 0; i < 5; i++)
        {
            if((users[0][pos].best_5_real[i]->coin_index)>=0)
            {
                proposals = 1;
                printf("   %s", users[0][pos].best_5_real[i]->name);
            }
        }

        if(!proposals)
        {
            printf("    none");
        }

        //with virtual users
        proposals = 0;
        printf("    ---||---");
        for(int i = 0; i < 2; i++)
        {
            if((users[0][pos].best_2_represent[i]->coin_index)>=0   )
            {
                proposals = 1;
                printf("   %s", users[0][pos].best_2_represent[i]->name);
            }
        }

        if(!proposals)
        {
            printf("    none");
        }



    }

    printf("\nExecution Time: <%.10lf>\n", recom_time);
}

// delete
struct users* delete_users(struct users* users[], long long users_num, int coins_num)
{   
    //printf("\n%d\n", __LINE__);

    for (long long pos = 0; pos < users_num; pos++)
    {   
        
        for (int y = 0; y < coins_num; y++)
        {   
            if((users[0][pos].user_crypto[0])!=NULL)
            {
                free(users[0][pos].user_crypto[0]);
            }
            users[0][pos].user_crypto[0] = NULL;
        }
        
        users[0][pos].first_tweet = NULL;
        users[0][pos].last_tweet = NULL;
        users[0][pos].coins = NULL;
        users[0][pos].coins_last_node = NULL;

        //printf("\n%d\n", __LINE__);

        for(int y = 0; y < 5; y++)
        {
            free(users[0][pos].best_5_real[y]);
            if(y < 2)
            {
                free(users[0][pos].best_2_represent[y]);
            }
        }
       //printf("\n%d\n", __LINE__);


        users[0][pos].best_5_real[0] = NULL;
        users[0][pos].best_2_represent[0] = NULL;
        
    }

    free(users[0]);
    
    return NULL;
}


void delete_sentim_node(struct sentim_score_map* sentim_score[], long long pos)
{
    free( sentim_score[0][pos].str );
    sentim_score[0][pos].str  = NULL;
}


void delete_sentim_map(struct sentim_score_map* sentim_score[], long long sentim_dictionary_size)
{
    for (long long i = 0; i < sentim_dictionary_size; i++)
    {
        delete_sentim_node(sentim_score, i);
    }

    free(sentim_score[0]);
    sentim_score[0] = NULL;

}

void delete_coin_node(struct crypto_coins* coins[], int pos)
{
    struct coin_ref_list* templist = NULL, *templist2 = NULL;
    templist = coins[0][pos].ref_list;
    while(templist!=NULL)
    {
        templist2 = templist;
        templist = templist -> next;  

        free(templist2 -> coin_ref);
        templist2 -> coin_ref = NULL;
        templist2 -> next = NULL;
        free(templist2);
        templist2 = NULL;

    }
    free(coins[0][pos].coin_refs[0]);

    coins[0][pos].ref_list = NULL;
    coins[0][pos].last_node = NULL;

}
    
void delete_coins(struct crypto_coins* coins[], int crypto_dictionary_size)
{
    for (long long i = 0; i < crypto_dictionary_size; i++)
    {
        delete_coin_node(coins, i);
    }
    free(coins[0]);
    coins[0] = NULL;
    
}

void delete_tweet_node(struct tweets* tweets[], long long tweet_num)
{


    struct tweet_content* content_temp = NULL, *content_temp_prev = NULL ;
    content_temp = tweets[0][tweet_num].content;
    while(content_temp!=NULL)
    {
        content_temp_prev = content_temp;
        content_temp = content_temp -> next;

        free(content_temp_prev -> word);
        content_temp_prev -> word = NULL;
        content_temp_prev -> next = NULL;
        free(content_temp_prev);
        content_temp_prev = NULL;
        

    }
    tweets[0][tweet_num].content = NULL;
    tweets[0][tweet_num].last_word = NULL;

    struct coins_in_tweet* coins_temp = NULL, *coins_temp_prev = NULL;
    coins_temp = tweets[0][tweet_num].coins;
    while(coins_temp!=NULL)
    {
        coins_temp_prev = coins_temp;
        coins_temp = coins_temp -> next;  

        //free(coins_temp_prev -> name);
        coins_temp_prev -> name = NULL;
        coins_temp_prev -> next = NULL;
        free(coins_temp_prev);
        coins_temp_prev = NULL;
        
        
    }
    tweets[0][tweet_num].coins = NULL;
    tweets[0][tweet_num].coins_last_node = NULL;


}

void delete_tweets(struct tweets* tweets[], long long tweets_size)
{
    for (long long i = 0; i < tweets_size; i++)
    {
        delete_tweet_node(tweets, i);

    }
    free(tweets[0]);
    tweets[0] = NULL;
}

void delete_data_structures(struct tweets* tweets[], long long tweets_num, struct sentim_score_map* sentim_score[], long long sentim_dictionary_size, struct crypto_coins* coins[], int crypto_dictionary_size)
{
    delete_sentim_map(sentim_score, sentim_dictionary_size);
    //printf("\nrecom_structs.c:%d\n", __LINE__);
    delete_coins(coins, crypto_dictionary_size);
    //printf("\nrecom_structs.c:%d\n", __LINE__);
    delete_tweets(tweets, tweets_num);
    //printf("\nrecom_structs.c:%d\n", __LINE__);
    printf("\n\nMemory Freed!\n");

}
