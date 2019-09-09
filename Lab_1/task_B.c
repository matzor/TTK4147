#include <assert.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>
#include <x86intrin.h>

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

struct timespec timespec_sub(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec - rhs.tv_sec, lhs.tv_nsec - rhs.tv_nsec);
}

struct timespec timespec_add(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec + rhs.tv_sec, lhs.tv_nsec + rhs.tv_nsec);
}

int timespec_cmp(struct timespec lhs, struct timespec rhs){
    if (lhs.tv_sec < rhs.tv_sec)
        return -1;
    if (lhs.tv_sec > rhs.tv_sec)
        return 1;
    return lhs.tv_nsec - rhs.tv_nsec;
}



int main(){

    struct timespec current_time;
    struct timespec then_time;
    struct tms time_buf;

    for(int i = 0; i < 10*1000*1000; i++){
        //clock_gettime(CLOCK_MONOTONIC, &current_time); //Average time 2.2*10^⁻8
        //uint time_start = __rdtsc(); //Averager time 8.8*10^-9
        times(&time_buf); //Average time 2.581*10^-7

    }

    double cpu_freq_GHz = 2.660091;
    int ns_max = 400;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);


    clock_t t1;
    clock_t t2;

    int oneSecond = sysconf(_SC_CLK_TCK);
    /*
    for(int i = 0; i < 10*1000*1000; i++){


        // t1 = timer()
        // t2 = timer()

        //clock_gettime
        /*
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        clock_gettime(CLOCK_MONOTONIC, &then_time);
        uint t1 = current_time.tv_nsec;
        uint t2 = then_time.tv_nsec;
        int ns =  (t2 - t1);
        */

        //__rdtsc
        /*
        uint t1 = __rdtsc();
        uint t2 = __rdtsc();
        int ns =  (t2 - t1) / (cpu_freq_GHz);
        */

        //times()
        /*
        t1 = times(&time_buf); //Average time 2.6*10^-9
        t2 = times(&time_buf); //Average time 2.6*10^-9
        int ns =  (int)((double)(t2-t1) * 1e9 / oneSecond);
        */
        /*
        //sched_yield
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        sched_yield();
        clock_gettime(CLOCK_MONOTONIC, &then_time);
        uint t1 = current_time.tv_nsec;
        uint t2 = then_time.tv_nsec;
        int ns =  (t2 - t1);

        if(ns >= 0 && ns < ns_max){
            histogram[ns]++;
        }
    }

    for(int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }
    */



    return 0;
}
