#include <pthread.h>
#include <stdio.h>

// Note the argument and return types: void*
void* fn(void* args){
  long local = 0;
    for(int i = 0; i < 50000000; i++){
      *args++;
      local++;
    }
    printf("Global %li \n Local %li \n" , args, local)
    return NULL;
}

int main(){
    long a = 0;
    pthread_t threadHandle, threadHandle2;
    pthread_create(&threadHandle, NULL, fn, NULL);
    pthread_create(&threadHandle2, NULL, fn, NULL);
    pthread_join(threadHandle, NULL);
    pthread_join(threadHandle2, NULL);
}
