#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>

void *chose_rand(void *num){
  *(int *) num = rand();
}

int main(int argc, char **argv){
  srand(time(NULL));
  const int N = atoi(*(argv+1));
  int res[N];
  pthread_t *randi = malloc(N* sizeof(pthread_t) );

  for(int i=0; i<N; i++){
    if(pthread_create(&randi[i], NULL, chose_rand, (void*) res+i)){
      perror("ERR: CAN'T CREATE THREAD");
      exit(0);
    }
  }

  for(int i=0; i<N; i++){
    if(pthread_join(randi[i], NULL)){
      perror("ERR: CAN'T JOIN THREAD");
      exit(0);
    }
  }

  for(int i = 0; i < N; i++) {    
      for(int j = i + 1; j < N; j++) {    
          if(res[i] == res[j])    
              printf("%d\n", res[j]);    
      }    
  }
  
  free(randi);
  return 0;
}