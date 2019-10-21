#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <rtdk.h>

#include "io.h"

typedef struct Args HandlerArgs;
struct Args {
	int channel;
};



int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

void* disturbance(void){
	set_cpu(0);
	while(1){
		asm volatile("" ::: "memory");
	}
}

void* responseTask(void* c){
	set_cpu(0);
	// Busy-polling that won't crash your computer (hopefully)
	unsigned long duration = 10000000000;  // 10 second timeout
	unsigned long endTime = rt_timer_read() + duration;

	HandlerArgs args = *(HandlerArgs*) c;

	while(1){

		if(io_read(args.channel) == 0){
			io_write(args.channel, 0);
			rt_timer_spin(5000);
			io_write(args.channel, 1);
		}
		
		if(rt_timer_read() > endTime){
		    rt_printf("Time expired\n");
		    rt_task_delete(NULL);
		}
		if(rt_task_yield()){
		    rt_printf("Task failed to yield\n");
		    rt_task_delete(NULL);
		}
	}
}



int main(){

	// Call these at the start of main
	rt_print_auto_init(1);
	mlockall(MCL_CURRENT | MCL_FUTURE);
	
	io_init();
	
	RT_TASK task_a , task_b, task_c;
	pthread_t random_threads[10];
	
	int a = 1;
	int b = 2;
	int c = 3;
	
	rt_task_create(&task_a, NULL, 0, 90, T_CPU(0));
	rt_task_create(&task_b, NULL, 0, 90, T_CPU(0));
	rt_task_create(&task_c, NULL, 0, 90, T_CPU(0));
	
	for(int i = 0; i<10; i++) {
		pthread_create(&random_threads[i], NULL, (void *)disturbance, NULL);
	}
	
	rt_task_start(&task_a, (void*)responseTask, &(HandlerArgs){1});
	rt_task_start(&task_b, (void*)responseTask, &(HandlerArgs){2});
	rt_task_start(&task_c, (void*)responseTask, &(HandlerArgs){3});
	
	for(int i = 0; i<10; i++) {
		  pthread_join(random_threads[i], NULL);
	}
	
	while(1)
	{
		usleep(-1);
	}
	
	
	return 0;
	
}

