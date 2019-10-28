#include <native/task.h>
#include <native/timer.h>
#include <native/mutex.h>
#include <sys/mman.h>
#include <rtdk.h>
#include <pthread.h>

#define HIGH 80
#define MEDIUM 50
#define LOW 10

#define TIME_UNIT 100

RT_MUTEX mutex_A;
RT_MUTEX mutex_B;
RT_MUTEX_INFO info_A;
RT_MUTEX_INFO info_B;
RT_TASK task_l , task_m, task_h;

void busy_wait_us(unsigned long delay){
		for(; delay > 0; delay--){
			rt_timer_spin(1000);
		}
}

void low_function(void){
	//while(1){
		rt_mutex_acquire(&mutex_A, TM_INFINITE); //TAR A

		rt_task_set_priority(&task_l, HIGH+1);

		rt_printf("LOW THREAD HAS STARTED! \n");
		busy_wait_us(3*TIME_UNIT);

		rt_mutex_acquire(&mutex_B, TM_INFINITE); //TAR B
		busy_wait_us(3*TIME_UNIT);

		rt_mutex_release(&mutex_B);
		rt_mutex_release(&mutex_A);

		rt_task_set_priority(&task_l, LOW);	

		busy_wait_us(1*TIME_UNIT);
		rt_printf("LOW THREAD HAS RUN! \n");
	//}
}


void high_function(void){
	//while(1){
		rt_task_sleep(TIME_UNIT * 1000);
		rt_mutex_acquire(&mutex_B, TM_INFINITE); //TAR B

		rt_printf("HIGH THREAD HAS STARTED! \n");
		busy_wait_us(1*TIME_UNIT);
		rt_mutex_acquire(&mutex_A, TM_INFINITE); //TAR A
		busy_wait_us(2*TIME_UNIT);
		rt_mutex_release(&mutex_A);
		rt_mutex_release(&mutex_B);
		busy_wait_us(1*TIME_UNIT);
		rt_printf("HIGH THREAD HAS RUN! \n");
	//}
}

int main(){
	mlockall(MCL_CURRENT | MCL_FUTURE);
	//rt_task_shadow(NULL, NULL, 90, T_CPU(0));
	rt_print_auto_init(1);
	rt_mutex_create(&mutex_A, NULL);
	rt_mutex_create(&mutex_B, NULL);
	rt_task_create(&task_l, NULL, 0, LOW, T_CPU(0));
	rt_task_create(&task_h, NULL, 0, HIGH, T_CPU(0));
	rt_task_start(&task_l, (void*)low_function, NULL);
	rt_task_start(&task_h, (void*)high_function, NULL);
	rt_task_sleep(100000000);
	//rt_sem_broadcast(&sem1);
	rt_task_sleep(100000000);

	while(1);

	rt_mutex_delete(&mutex_A);
	rt_mutex_delete(&mutex_B);
	return 0;
}
