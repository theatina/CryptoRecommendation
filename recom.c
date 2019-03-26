//
//  recom.c
//
//  Christina-Theano Kylafi
//  AM: 1115201200077


#include "recom.h"

int main(int argc, char* argv[])
{

    //rand seed for later
	struct timeval time; 
    gettimeofday(&time,NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

    char inputfile[40];
    memset((void*)inputfile, 0, 40*sizeof(char));
	int inputflag = 0;

	char outputfile[40];
    memset((void*)outputfile, 0, 40*sizeof(char));
	int outputflag = 0;

	//int metric = 2;  //2 for cosine
    int p_nearest = default_p_nearest;

    int check_pars = 0;
    int validation = 0;
	if((check_pars = init_params_recom(argc, argv, &inputflag, inputfile, &outputflag, outputfile, &validation))==-99)
	{
		//print_msg_centre_line("CLUSTERIN' OUT!!");
		printf("\n\n");
		return -999;
	}
    //if not exists, create the output file
    FILE* temp = NULL;
	temp = fopen(outputfile, "w+");
	fclose(temp);

    print_welcome_msg_recom();

    //save data
    char sentim_dictionary_name[] = "vader_lexicon.csv";
    long long sentim_dictionary_size = 0;
    sentim_dictionary_size = count_lines(sentim_dictionary_name);
    printf("L_Dictionary: %lld lines\n", sentim_dictionary_size);

    char crypto_dictionary_name[] = "coins_queries.csv";
    long long crypto_dictionary_size = 0;
    crypto_dictionary_size = count_lines(crypto_dictionary_name);
    printf("K_Dictionary: %lld lines\n", crypto_dictionary_size);

    //structures
    //map with sentiment score
    struct sentim_score_map* sentim_score[1];  //the structure of clusters
	sentim_score[0] = malloc( sentim_dictionary_size * get_size_sentim_score_map());
    save_data_sentim_score_map(sentim_score, sentim_dictionary_name);
    //print_sentim_score_map(sentim_score, sentim_dictionary_size);
    
    //crypto currency table
    struct crypto_coins* coins[1];  //the structure of clusters
	coins[0] = malloc( crypto_dictionary_size * get_size_crypto_coins());
    save_data_crypto_coins(coins, crypto_dictionary_name);
    //print_crypto_coins(coins, crypto_dictionary_size);
    //sleep(10);
    crypto_coins_create_array(coins, crypto_dictionary_size);
    //print_crypto_coins_array(coins, crypto_dictionary_size);
    
    //tweets as in inputfile
    struct tweets* tweets[1]; 
    long long tweets_size = count_lines(inputfile)-1;
    printf("\nTweets: %lld\n",tweets_size );
	tweets[0] = malloc( tweets_size * get_size_tweets());
    tweets_init(tweets, tweets_size);
    long long users_num = 0;
    users_num = save_data_tweets(tweets, inputfile, coins, crypto_dictionary_size, &p_nearest);
    //print_tweets(tweets, tweets_size, coins);
   
    //users
    struct users* users[1]; 
	users[0] = malloc( users_num * get_size_users());
    users_init(users, users_num, crypto_dictionary_size);
    printf("\n\nTotal Users: %lld\n", users_num);

    //starting the process
    if(!validation)
    {
        recom_steps(tweets, tweets_size, sentim_score, sentim_dictionary_size, coins, crypto_dictionary_size, users, users_num, outputfile, p_nearest);
    //print_tweets(tweets, tweets_size, coins);
    //print_users(users, users_num, crypto_dictionary_size, coins);
        delete_data_structures(tweets, tweets_size, sentim_score, sentim_dictionary_size, coins, crypto_dictionary_size);

    }
    else if(validation == 1)
    {
        validation_algo(tweets, tweets_size, sentim_score, sentim_dictionary_size, coins, crypto_dictionary_size, users, users_num, outputfile, p_nearest);
        delete_data_structures(tweets, tweets_size, sentim_score, sentim_dictionary_size, coins, crypto_dictionary_size);


    }

    //deletes

    printf("\n");
    print_msg_centre_line(" RECOMMENDin' OUT ! ");
	printf("\n\n");
    return 9;
}