#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#include <gmp.h>
#include <string.h>

struct num_ext{// num_dec=num-1 = mant* 2^exp
  mpz_t num;
  mpz_t num_dec;
  mpz_t num_range;
  mpz_t mant;
  unsigned long exp;
  bool primality;
};
struct num_ext subject;

pthread_mutex_t lock; // might not be necessary
gmp_randstate_t rstate;


void *witness_test(void){
	mpz_t base, x;
	mpz_inits(base, x, NULL);
	mpz_urandomm(base, rstate, subject.num_range);
	mpz_add_ui(base, base, 2); // this and num_range can be avoided for large enough subject.num since the probability of mpz_urandom returning 0,1 or num-1, is very small 
	mpz_powm(x, base, subject.mant, subject.num);

	if ((mpz_cmp_ui(x, 1)==0)||(mpz_cmp(x, subject.num_dec)==0)){
		mpz_clears(base, x, NULL);
		return NULL;
	}

	for(unsigned long i=1; i<subject.exp; i++){
		mpz_powm_ui(x, x, 2, subject.num);
		if ((mpz_cmp_ui(x, 1)==0)||(mpz_cmp(x, subject.num_dec)==0)){
			mpz_clears(base, x, NULL);
			return NULL;
		}
	}

	pthread_mutex_lock(&lock);
	subject.primality = false;
	pthread_mutex_unlock(&lock);
	mpz_clears(base, x, NULL);
	return NULL;
}

int main(int argc, char **argv){
	if (argc < 3) 
		return 0;

	int nb_witness;
	unsigned long seed = (unsigned long) time(NULL);
	nb_witness = atoi(*(argv +2));

	mpz_inits(subject.num, subject.num_dec, subject.num_range, subject.mant, NULL);
	gmp_randinit_default(rstate);
	gmp_randseed_ui(rstate, seed);

	mpz_set_str(subject.num, *(argv+1), 10);
	mpz_sub_ui(subject.num_dec, subject.num, 1);
	mpz_sub_ui(subject.num_range, subject.num, 3);
	subject.exp = mpz_scan1(subject.num_dec, 0);
	mpz_cdiv_q_2exp (subject.mant, subject.num_dec, subject.exp);
	subject.primality = true;

	if (mpz_even_p(subject.num) || 	mpz_cmp_ui(subject.num, 3)<=0){
		printf("The number is%s prime.\n", (strlen(*(argv+1))==1 && (**(argv+1)==50 || **(argv+1)==51)) ? "" : " not");
		return 0;
	}
 
	pthread_t *witness = malloc(sizeof(pthread_t)*nb_witness);

	if (pthread_mutex_init(&lock, NULL) != 0) {
	      printf("ERR: MUTEX INIT FAILED\n");
	      exit(0);
	  }

	for(int i=0; i<nb_witness; i++){
		if(pthread_create(&witness[i], NULL, witness_test, NULL)){
			perror("ERR: CAN'T CREATE THREAD");
			exit(0);
		}
	}

	for(int i=0; i<nb_witness; i++){
		if(pthread_join(witness[i], NULL)){
			perror("ERR: CAN'T JOIN THREAD");
			exit(0);
		}
	}

	printf("The number is%s prime.\n", (subject.primality)?" probably":" not");

	mpz_clears(subject.num, subject.num_dec, subject.num_range, subject.mant, rstate, NULL);
	free(witness);
	
  return 0;
}