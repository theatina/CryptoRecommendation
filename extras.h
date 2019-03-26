//
//  extras.h
//
//  Christina-Theano Kylafi
//  AM: 1115201200077

#ifndef extras_h
#define extras_h


#include "recom.h"

int check_if_same_str(char* str1, char* str2);
long long count_lines(const char file[]);
int init_params_recom(int argc, char* argv[], int* inputflag, char* inputfile, int* outputflag, char*outputfile, int* validation);
void write_to_outputfile_recom(char* outputfile, struct users* users[], long long users_num, double recom_time, int problem);

#endif /* extras_h */