#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <unistd.h>

#include "io.h"


struct timespec timespec_normalized(time_t sec, long nsec){
    while(nsec >= 1000000000){
        nsec -= 1000000000;
        ++sec;
    }
    while(nsec < 0){
        nsec += 1000000000;
        --sec;
    }
    return (struct timespec){sec, nsec};
}

struct timespec timespec_add(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec + rhs.tv_sec, lhs.tv_nsec + rhs.tv_nsec);
}

int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

void* responseTask(int* channel){
  set_cpu(0);

	struct timespec waketime;
	clock_gettime(CLOCK_REALTIME, &waketime);

	struct timespec period = {.tv_sec = 0, .tv_nsec = 1*100*1000};

	while(1){
			if(io_read(*channel)==0){
				io_write(*channel, 0);
				usleep(5);
				io_write(*channel, 1);
			}

	    // sleep
	    waketime = timespec_add(waketime, period);
	    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
	}
}

void* disturbance(void){
	set_cpu(0);
	while(1){
		asm volatile("" ::: "memory");
	}
}

int main(){
  io_init();
  pthread_t task_a, task_b, task_c;
	pthread_t random_threads[10];
  int a, b, c;
  a = 1; b = 2; c = 3;

  pthread_create(&task_a, NULL, (void *)responseTask, &a);
  pthread_create(&task_b, NULL, (void *)responseTask, &b);
  pthread_create(&task_c, NULL, (void *)responseTask, &c);

	for(int i = 0; i<10; i++) {
		pthread_create(&random_threads[i], NULL, (void *)disturbance, NULL);
	}

  pthread_join(task_a, NULL);
  pthread_join(task_b, NULL);
  pthread_join(task_c, NULL);

	for(int i = 0; i<10; i++) {
		  pthread_join(random_threads[i], NULL);
	}

}
