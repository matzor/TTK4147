#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <unistd.h>

#include "io.h"

int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

void* responseTask(int* channel){
  set_cpu(0);
  while(1){
    if(io_read(*channel)==0){
      io_write(*channel, 0);
      usleep(5);
      io_write(*channel, 1);
    }
  }
}

int main(){
  io_init();
  pthread_t task_a, task_b, task_c;
  int a, b, c;
  a = 1; b = 2; c = 3;
  pthread_create(&task_a, NULL, responseTask, &a);
  pthread_create(&task_b, NULL, responseTask, &b);
  pthread_create(&task_c, NULL, responseTask, &c);
  pthread_join(task_a, NULL);
  pthread_join(task_b, NULL);
  pthread_join(task_c, NULL);
}
