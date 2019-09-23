#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define N 5
pthread_t philospher[N+1];
pthread_mutex_t spoon[N+1];

int spoonFlag[N];

void* process(int j){
    int i = j;
    while (1){
      printf("Philospher %i is waiting for spoons\n", i);
      pthread_mutex_lock(&spoon[i-1]);
      spoonFlag[i-1] = 1;
      usleep(50000);
      if(spoonFlag[i%N] == 1){
        spoonFlag[i-1] = 0;
        pthread_mutex_unlock(&spoon[i-1]);
        continue;
      }
      pthread_mutex_lock(&spoon[i%N]); //fixes looparound
      spoonFlag[i%N] = 1;
      printf("Philospher %i got both spoons!\n", i);
      usleep(i * 100000);
      pthread_mutex_unlock(&spoon[i-1]);
      spoonFlag[i-1] = 0;
      pthread_mutex_unlock(&spoon[i%N]);
      spoonFlag[i%N] = 0;
      usleep(i * 500000);
    }
    return NULL;
}


int main(){
    for (int i = 0; i < N; i++){
        spoonFlag[i] = 0;
    }

    for (int i = 0; i < N; i++){
      pthread_mutex_init(&spoon[i], NULL);
    }

    int j = 1;
    for (j = 1; j <= N; j++){
      pthread_create(&philospher[j], NULL, (void *)process, (int*)j);
    }

    for (int i = 1; i <= N; i++){
      pthread_join(philospher[i], NULL);
    }

    //pthread_mutex_destroy(&mtx);
    return 0;
}
