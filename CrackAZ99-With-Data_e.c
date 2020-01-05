#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include<pthread.h>

/***********************************************************************
*******
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2
uppercase
  letters and a 2 digit integer. Your personalised data set is included
in the
  code.

  Compile with:
    cc -o CrackAZ99-With-Data CrackAZ99-With-Data.c -lcrypt -pthread

  If you want to analyse the results then use the redirection operator
to send
  output to a file that you can view using an editor or the less
utility:

    ./CrackAZ99-With-Data > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
************************************************************************
******/
int n_passwords = 4;

char *encrypted_passwords[] = {

"$6$KB$0G24VuNaA9ApVG4z8LkI/OOr9a54nBfzgQjbebhqBZxMHNg0HiYYf1Lx/HcGg6q1nnOSArPtZYbGy7yc5V.wP/",
"$6$KB$VDUCASt5S88l82JzexhKDQLeUJ5zfxr16VhlVwNOs0YLiLYDciLDmN3QYAE80UIzfryYmpR.NFmbZvAGNoaHW",
"$6$KB$0n1YjoLnJBuAdeBsYFW3fpZzMPP8xycQbEj35GvoerMnEkWIAKnbUBAb70awv5tfHylWkVzcwzHUNy/7l7I1c/",
"$6$KB$HKffNNiGzngqYueF89z3gwWZMg.xUBIz/00QSCbgwKtRHmwUbZX6jTH4VUAg3L3skaO8qtNf5LE7WP39jQ7ZJ0ter"
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All
combinations
 that are tried are displayed and when the password is found, #, is put
at the
 start of the line. Note that one of the most time consuming operations
that
 it performs is the output of intermediate results, so performance
experiments
 for this kind of program should not include this. i.e. comment out the
printfs.
*/

void crack(char *encrypted_passwords){
  pthread_t t1,t2;
  int i;
  void *first_block();
  void *second_block();
    
         pthread_create(&t1,NULL,first_block,(void *)encrypted_passwords);
         pthread_create(&t2,NULL,second_block,(void *)encrypted_passwords);
  

         pthread_join(t1,NULL);
         pthread_join(t2,NULL);
 
}
void *first_block(char *salt_and_encrypted){
int m,n,o; //loop counters
char salt[7];
char plain[7];//The combination of letters currently being checked
char *enc;//Pointer to the encrypted password
int count =0; //The number of combinations explored so far

substr(salt,salt_and_encrypted,0,6);

  for(m='A'; m<='M'; m++){
    for(n='A'; n<='Z'; n++){
      for(o=0; o<=99; o++){
              
        sprintf(plain, "%c%c%02d", m, n, o); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } //else {
          //printf(" %-8d%s %s\n", count, plain, enc);
        //}
       }
      }
    }
  
  printf("%d solutions explored\n", count);
}
void *second_block(char *salt_and_encrypted){
int m,n,o; //loop counters
char salt[7];
char plain[7];//The combination of letters currently being checked
char *enc;//Pointer to the encrypted password
int count =0; //The number of combinations explored so far

substr(salt,salt_and_encrypted,0,6);

  for(m='N'; m<='Z'; m++){
    for(n='A'; n<='Z'; n++){
      for(o=0; o<=99; o++){
              
        sprintf(plain, "%c%c%02d", m, n, o); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
          
        } //else {
          //printf(" %-8d%s %s\n", count, plain, enc);
        //}
       }
      }
    
  }
  printf("%d solutions explored\n", count);
}



//Calculating time

int time_difference(struct timespec *start, struct timespec *finish, long long int *difference)
 {
	  long long int ds =  finish->tv_sec - start->tv_sec; 
	  long long int dn =  finish->tv_nsec - start->tv_nsec; 

	  if(dn < 0 ) {
	    ds--;
	    dn += 1000000000; 
  } 
	  *difference = ds * 1000000000 + dn;
	  return !(*difference > 0);
}
int main(int argc, char *argv[])
{
  	int i;
	struct timespec start, finish;   
  	long long int time_elapsed;

  	clock_gettime(CLOCK_MONOTONIC, &start);

  	for(i=0;i<n_passwords;i<i++) 
	{
    		crack(encrypted_passwords[i]);
  	}
	clock_gettime(CLOCK_MONOTONIC, &finish);
	  time_difference(&start, &finish, &time_elapsed);
	  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
		                                 (time_elapsed/1.0e9)); 
  return 0;
}
