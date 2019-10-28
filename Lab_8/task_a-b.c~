#include <native/task.h>
#include <native/timer.h>
#include <native/sem.h>
#include <sys/mman.h>
#include <rtdk.h>
#include <pthread.h>

#define HIGH 80
#define MEDIUM 50
#define LOW 10

#define TIME_UNIT 100

RT_SEM sem1;

void busy_wait_us(unsigned long delay){
		for(; delay > 0; delay--){
			rt_timer_spin(1000);
		}
}

void low_function(void){
	//while(1){
		rt_sem_p(&sem1, TM_INFINITE);
		rt_printf("LOW THREAD HAS STARTED! \n");
		busy_wait_us(3*TIME_UNIT);
		rt_sem_v(&sem1);
		rt_printf("LOW THREAD HAS RUN! \n");
	//}
}

void medium_function(void){
	//while(1){
		rt_task_sleep(TIME_UNIT * 1000);
		rt_printf("MEDIUM THREAD HAS STARTED! \n");
		busy_wait_us(5*TIME_UNIT);
		rt_printf("MEDIUM THREAD HAS RUN! \n");
	//}
}

void high_function(void){
	//while(1){
		rt_task_sleep(TIME_UNIT * 2000);
		rt_sem_p(&sem1, TM_INFINITE);
		rt_printf("HIGH THREAD HAS STARTED! \n");
		busy_wait_us(2*TIME_UNIT);
		rt_sem_v(&sem1);
		rt_printf("HIGH THREAD HAS RUN! \n");
	//}
}

int main(){
	mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_task_shadow(NULL, NULL, 90, T_CPU(0));
	rt_print_auto_init(1);
	rt_sem_create(&sem1, NULL, 1, NULL);
	RT_TASK task_l , task_m, task_h;
	rt_task_create(&task_l, NULL, 0, LOW, T_CPU(0));
	rt_task_create(&task_m, NULL, 0, MEDIUM, T_CPU(0));
	rt_task_create(&task_h, NULL, 0, HIGH, T_CPU(0));
	rt_task_start(&task_l, (void*)low_function, NULL);
	rt_task_start(&task_m, (void*)medium_function, NULL);
	rt_task_start(&task_h, (void*)high_function, NULL);
	rt_task_sleep(100000000);
	//rt_sem_broadcast(&sem1);
	rt_task_sleep(100000000);

	rt_sem_delete(&sem1);
	return 0;
}
