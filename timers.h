#include<time.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<sys/types.h>
#include<unistd.h>

#define TIMER_START MONO_HOT_START

#define TIMER_STOP MONO_HOT_STOP

// Our original timer function without proper serialization
#define ASM_BASE_START \
	int t1, t2, t3, t4;\
	asm("rdtscp; mov %%eax, %0;":"=a"(t1));\
	asm(        "mov %%edx, %0;":"=a"(t2));

#define ASM_BASE_STOP \
	asm("rdtscp; mov %%eax, %0;":"=a"(t3));\
	asm(        "mov %%edx, %0;":"=a"(t4));\
	long timer_result = ((long)(t4 - t2) << 32) + ((unsigned long)(t3 - t1) | 0x0000FFFF);

// Our original timer with a trailing for loop to show unordered execution
#define ASM_BASE_FOR_START \
	int t1, t2, t3, t4;\
	asm("rdtscp; mov %%eax, %0;":"=a"(t1));\
	asm(        "mov %%edx, %0;":"=a"(t2));

#define ASM_BASE_FOR_STOP \
	asm("rdtscp; mov %%eax, %0;":"=a"(t3));\
	asm(        "mov %%edx, %0;":"=a"(t4));\
   volatile int ti;\
   volatile int tj;\
   for (ti = 0; ti < 5; ti++) {\
      tj = ti;\
   }\
	long timer_result = ((long)(t4 - t2) << 32) + ((unsigned long)(t3 - t1) | 0x0000FFFF);

// The suggested Intel timer with proper serialization and a cold cache
#define ASM_COLD_START \
	int t1, t2, t3, t4;\
   asm("CPUID;" \
	    "rdtsc;"\
       "mov %%eax, %0;"\
       "mov %%edx, %1;"\
      :"=r"(t1), "=r"(t2)\
     ::"%eax", "%ebx", "%ecx", "%edx");

#define ASM_COLD_STOP \
   asm("rdtscp;"\
       "mov %%eax, %0;"\
       "mov %%edx, %1;"\
       "cpuid;"\
      :"=r"(t3), "=r"(t4)\
     ::"%eax", "%ebx", "%ecx", "%edx");\
	long timer_result = ((long)(t4 - t2) << 32) + ((unsigned long)(t3 - t1) | 0x0000FFFF);


// The suggested Intel timer with proper serialization and a warm cache
#define ASM_HOT_START \
	int t1, t2, t3, t4;\
   asm("CPUID;" \
	    "rdtsc;"\
       "mov %%eax, %0;"\
       "mov %%edx, %1;"\
      :"=r"(t1), "=r"(t2)\
     ::"%eax", "%ebx", "%ecx", "%edx");\
   asm("rdtscp;"\
       "mov %%eax, %0;"\
       "mov %%edx, %1;"\
       "cpuid;"\
      :"=r"(t3), "=r"(t4)\
     ::"%eax", "%ebx", "%ecx", "%edx");\
	asm("CPUID;" \
	    "rdtsc;"\
       "mov %%eax, %0;"\
       "mov %%edx, %1;"\
      :"=r"(t1), "=r"(t2)\
     ::"%eax", "%ebx", "%ecx", "%edx");\
   asm("rdtscp;"\
       "mov %%eax, %0;"\
       "mov %%edx, %1;"\
       "cpuid;"\
      :"=r"(t3), "=r"(t4)\
     ::"%eax", "%ebx", "%ecx", "%edx");\
	asm("CPUID;" \
	    "rdtsc;"\
       "mov %%eax, %0;"\
       "mov %%edx, %1;"\
      :"=r"(t1), "=r"(t2)\
     ::"%eax", "%ebx", "%ecx", "%edx");

#define ASM_HOT_STOP \
   asm("rdtscp;"\
       "mov %%eax, %0;"\
       "mov %%edx, %1;"\
       "cpuid;"\
      :"=r"(t3), "=r"(t4)\
     ::"%eax", "%ebx", "%ecx", "%edx");\
   long timer_result = ((long)(t4 - t2) << 32) + ((unsigned long)(t3 - t1) | 0x0000FFFF);


// The monotonic timer in NS, to compete with the asm strategies (cold cache).
#define MONO_COLD_START \
   struct timespec temp_ts_1, temp_ts_2;\
   clock_gettime(CLOCK_MONOTONIC, &temp_ts_1);

#define MONO_COLD_STOP \
   clock_gettime(CLOCK_MONOTONIC, &temp_ts_2);\
   long timer_result = ((temp_ts_2.tv_sec - temp_ts_1.tv_sec) * 1000000000 + temp_ts_2.tv_nsec - temp_ts_1.tv_nsec) * 3.392227000; 


// The monotonic timer in NS, to compete with the asm strategies (warm cache).
#define MONO_HOT_START \
   struct timespec temp_ts_1, temp_ts_2;\
   clock_gettime(CLOCK_MONOTONIC, &temp_ts_1);\
   clock_gettime(CLOCK_MONOTONIC, &temp_ts_1);\
   clock_gettime(CLOCK_MONOTONIC, &temp_ts_1);

#define MONO_HOT_STOP \
   clock_gettime(CLOCK_MONOTONIC, &temp_ts_2);\
   long timer_result = ((temp_ts_2.tv_sec - temp_ts_1.tv_sec) * 1000000000 + temp_ts_2.tv_nsec - temp_ts_1.tv_nsec) * 3.392227000; 

