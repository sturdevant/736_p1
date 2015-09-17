#include<stdio.h>

#include<time.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>
#include"output.h"
#include"arrays.h"

#define RUN_COUNT 200000
#define TIMER_COUNT 6

#define OPERATIONS_TO_TIME  

// Forward declarations for each of the timer functions.
long asm_base();
long asm_base_for();
long asm_cold();
long asm_hot();
long mono_cold();
long mono_hot();

int main(int argc, char** argv) {
	
	long t_result[TIMER_COUNT][RUN_COUNT];
	long (*timer_func[TIMER_COUNT])(void);
	timer_func[0] = &asm_base;
	timer_func[1] = &asm_base_for;
	timer_func[2] = &asm_cold;
  	timer_func[3] = &asm_hot;
   timer_func[4] = &mono_cold;
   timer_func[5] = &mono_hot;
	int i, j;

	for (i = 0; i < TIMER_COUNT; i++) {
		for (j = 0; j < RUN_COUNT; j++) {
			t_result[i][j] = timer_func[i]();
		}
	}

   array_to_csv(t_result[0], RUN_COUNT, "asm_base.csv");
   array_to_csv(t_result[1], RUN_COUNT, "asm_base_for.csv");
   array_to_csv(t_result[2], RUN_COUNT, "asm_cold.csv");
   array_to_csv(t_result[3], RUN_COUNT, "asm_hot.csv");
   array_to_csv(t_result[4], RUN_COUNT, "mono_cold.csv");
   array_to_csv(t_result[5], RUN_COUNT, "mono_hot.csv");

   for (i = 0; i < TIMER_COUNT; i++) {
      printf("Min #%d = %ld\n", i, array_min(&t_result[i][0], RUN_COUNT));
   }


	return 0;
}

// Our original timer function without proper serialization
long asm_base() {
	int t1, t2, t3, t4;
	asm("rdtscp; mov %%eax, %0;":"=a"(t1));
	asm(        "mov %%edx, %0;":"=a"(t2));
	OPERATIONS_TO_TIME
	asm("rdtscp; mov %%eax, %0;":"=a"(t3));
	asm(        "mov %%edx, %0;":"=a"(t4));
	return ((long)(t4 - t2) << 32) + t3 - t1;
}

// Our original timer with a trailing for loop to show unordered execution
long asm_base_for() {
	int t1, t2, t3, t4;
	asm("rdtscp; mov %%eax, %0;":"=a"(t1));
	asm(        "mov %%edx, %0;":"=a"(t2));
	OPERATIONS_TO_TIME
	asm("rdtscp; mov %%eax, %0;":"=a"(t3));
	asm(        "mov %%edx, %0;":"=a"(t4));
   volatile int i;
   volatile int j;
   for (i = 0; i < 5; i++) {
      j = i;
   }
   return ((long)(t4 - t2) << 32) + t3 - t1;
}

// The suggested Intel timer with proper serialization and a cold cache
long asm_cold() {
	int t1, t2, t3, t4;
   asm("CPUID;" 
	    "rdtsc;"
       "mov %%eax, %0;"
       "mov %%edx, %1;"
      :"=r"(t1), "=r"(t2)
     ::"%eax", "%ebx", "%ecx", "%edx");
	OPERATIONS_TO_TIME
   asm("rdtscp;"
       "mov %%eax, %0;"
       "mov %%edx, %1;"
       "cpuid;"
      :"=r"(t3), "=r"(t4)
     ::"%eax", "%ebx", "%ecx", "%edx");
	return ((long)(t4 - t2) << 32) + t3 - t1;
}

// The suggested Intel timer with proper serialization and a warm cache
long asm_hot() {
	int t1, t2, t3, t4;
   asm("CPUID;" 
	    "rdtsc;"
       "mov %%eax, %0;"
       "mov %%edx, %1;"
      :"=r"(t1), "=r"(t2)
     ::"%eax", "%ebx", "%ecx", "%edx");
   asm("rdtscp;"
       "mov %%eax, %0;"
       "mov %%edx, %1;"
       "cpuid;"
      :"=r"(t3), "=r"(t4)
     ::"%eax", "%ebx", "%ecx", "%edx");
	asm("CPUID;" 
	    "rdtsc;"
       "mov %%eax, %0;"
       "mov %%edx, %1;"
      :"=r"(t1), "=r"(t2)
     ::"%eax", "%ebx", "%ecx", "%edx");
   asm("rdtscp;"
       "mov %%eax, %0;"
       "mov %%edx, %1;"
       "cpuid;"
      :"=r"(t3), "=r"(t4)
     ::"%eax", "%ebx", "%ecx", "%edx");
	asm("CPUID;" 
	    "rdtsc;"
       "mov %%eax, %0;"
       "mov %%edx, %1;"
      :"=r"(t1), "=r"(t2)
     ::"%eax", "%ebx", "%ecx", "%edx");
	OPERATIONS_TO_TIME
   asm("rdtscp;"
       "mov %%eax, %0;"
       "mov %%edx, %1;"
       "cpuid;"
      :"=r"(t3), "=r"(t4)
     ::"%eax", "%ebx", "%ecx", "%edx");
	return (((long)(t4 - t2) << 32) + t3 - t1);
}


// The monotonic timer in NS, to compete with the asm strategies (cold cache).
long mono_cold() {
   struct timespec temp_ts_1, temp_ts_2;
   clock_gettime(CLOCK_MONOTONIC, &temp_ts_1);
   OPERATIONS_TO_TIME
   clock_gettime(CLOCK_MONOTONIC, &temp_ts_2);
   return (temp_ts_2.tv_nsec - temp_ts_1.tv_nsec) * 3.392227000; 
}

// The monotonic timer in NS, to compete with the asm strategies (warm cache).
long mono_hot() {
   struct timespec temp_ts_1, temp_ts_2;
   clock_gettime(CLOCK_MONOTONIC, &temp_ts_1);
   clock_gettime(CLOCK_MONOTONIC, &temp_ts_1);
   clock_gettime(CLOCK_MONOTONIC, &temp_ts_1);
   OPERATIONS_TO_TIME
   clock_gettime(CLOCK_MONOTONIC, &temp_ts_2);
   return (temp_ts_2.tv_nsec - temp_ts_1.tv_nsec) * 3.392227000; 
}
