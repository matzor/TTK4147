#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

long a = 0;
sem_t mutex;


// Note the argument and return types: void*
void* fn(void* args){
  long local = 0;
    for(int i = 0; i < 5000000; i++){
      // *args++;
      sem_wait(&mutex);
      a++;
      sem_post(&mutex);
      local++;
    }
    printf("Global %li \n Local %li \n" , a, local);
    return NULL;
}

int main(){
    sem_init(&mutex, 0, 1);
    pthread_t threadHandle, threadHandle2;
    pthread_create(&threadHandle, NULL, fn, &a);
    pthread_create(&threadHandle2, NULL, fn, &a);
    pthread_join(threadHandle, NULL);
    pthread_join(threadHandle2, NULL);
}
