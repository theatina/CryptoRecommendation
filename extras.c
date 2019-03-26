//
//  extras.c
//
//  Christina-Theano Kylafi
//  AM: 1115201200077


#include "extras.h"

//returns 1 if str1 is the same with str2
int check_if_same_str(char* str1, char* str2)
{
    int same = 0, size1 = 0, size2 = 0;
    size1 = strlen(str1);
    size2 = strlen(str2);
    
    if(size1 == size2 )
    {  if(!(strcmp(str1, str2)))
        {
                  
            same = 1;
              
        }
    }

    

    return same;

}

long long count_lines(const char file[])
{
    long long lines = 1;
    char ch = 'a', ch_save = 'b';
    FILE* fp = NULL;
    if ((fp = fopen(file, "r")) == NULL)
  	{ 	perror("\nfopen source-file"); printf("\n"); return -88; }
	
  	while(!feof(fp))   //count file lines
	{	
        ch_save = ch;
  		ch = fgetc(fp);
  		if(ch == '\n')
  		{	
            lines++;
		
  		}
	}
    //printf("\n Lines: |%c| \n", ch_save);
    if(ch_save == '\n')
    {
        lines--;
    }
    //printf("\nLines: %lld\n", lines);

	rewind(fp);

    return lines;
}

int init_params_recom(int argc, char* argv[], int* inputflag, char* inputfile, int* outputflag, char*outputfile, int* validation)
{

    if ((argc > 6) || (argc < 5)) // if not inserted as arguments, the programm will ask
    {
        fprintf(stderr,"\nUsage: %s -d <input_file> -o <output_file> -validate(for validation of results - optional)\n\n", argv[0]);
        return -99;
    }

    for (int i=1; i < argc; i++)
    {
        if (!(strcmp(argv[i], "-d")))
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
        else if(!(strcmp(argv[i], "-validate")))
        {
            (*validation) = 1;
        }
        
    }

    if(!(*inputflag) || !(*outputflag))
    {
    	printf("\nError\nSomething went wrong with the files!\n");
    	fprintf(stderr,"\nUsage: %s -d <input_file> -o <output_file> -validate(optional)\n\n", argv[0]);
        return -99;
    }

    return 999;
}


// void write_to_outputfile_recom(char* outputfile, struct users* users[], long long users_num,  double recom_time, int problem)
// {
//     FILE* fd = NULL;
//     fd = fopen(outputfile, "ab+");

//     if(problem == 11) //cosine-lsh - real users
//     {
//         fprintf(fd, "Cosine LSH - Real Users" );
//     }
//     else if(problem == 12) //cosine-lsh - representative users
//     {
//         fprintf(fd, "Cosine LSH - Representative Users" );
//     }
//     else if(problem == 21) //clustering - real userss
//     {
//         fprintf(fd, "Clustering - Real Users" );
//     }
//     else if(problem == 22) //cosine-lsh - representative users
//     {
//         fprintf(fd, "Clustering - Representative Users" );
//     }

//     for (long long k = 0; k < users_num; k++)
//     {
//         fprintf(fd, "\n<u%lld>", k);

//     }

    

// }
