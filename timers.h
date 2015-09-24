
// A variety of files will need a variety of includes. I put all of them here.

#include<time.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<limits.h>
#include<signal.h>
#include"arrays.h"
#include"output.h"


#define CLOCKS_PER_NS 3.192903

#define TIMER_START asm_hot_start()

#define TIMER_STOP asm_hot_stop()


double asm_base_start() {
   int t1, t2;
   asm("rdtscp; mov %%eax, %0;":"=a"(t1));
   asm(        "mov %%edx, %0;":"=a"(t2));
   return (double)((((unsigned long)t2) << 32) + ((unsigned long)(t1) & 0xFFFFFFFF)) / CLOCKS_PER_NS;
}

double asm_base_stop() {
   int t1, t2;
   asm("rdtscp; mov %%eax, %0;":"=a"(t1));
   asm(        "mov %%edx, %0;":"=a"(t2));
   return (double)((((unsigned long)t2) << 32) + ((unsigned long)(t1) & 0xFFFFFFFF)) / CLOCKS_PER_NS;
}

double asm_base_for_start() {
   int t1, t2;
   asm("rdtscp; mov %%eax, %0;":"=a"(t1));
   asm(        "mov %%edx, %0;":"=a"(t2));
   return (double)((((unsigned long)t2) << 32) + ((unsigned long)(t1) & 0xFFFFFFFF)) / CLOCKS_PER_NS;
}

double asm_base_for_stop() {
   int t1, t2;
   asm("rdtscp; mov %%eax, %0;":"=a"(t1));
   asm(        "mov %%edx, %0;":"=a"(t2));
   volatile int i;
   volatile int j;
   for (i = 0; i < 20; i++) {
      j *= i + 1;
   }
   return (double)((((unsigned long)t2) << 32) + ((unsigned long)(t1) & 0xFFFFFFFF)) / CLOCKS_PER_NS;
}

double asm_cold_start() {
   int t1, t2;
   asm("cpuid;"
       "rdtsc;"
       "mov %%eax, %0;"
       "mov %%edx, %1;"
      :"=r"(t1), "=r"(t2)
     ::"%eax", "%ebx", "%ecx", "%edx");
   return (double)((((unsigned long)t2) << 32) + ((unsigned long)(t1) & 0xFFFFFFFF)) / CLOCKS_PER_NS;
}

double asm_cold_stop() {
   int t1, t2;
   asm("rdtscp;"
       "mov %%eax, %0;"
       "mov %%edx, %1;"
       "cpuid;"
      :"=r"(t1), "=r"(t2)
     ::"%eax", "%ebx", "%ecx", "%edx");
   return (double)((((unsigned long)t2) << 32) + ((unsigned long)(t1) & 0xFFFFFFFF)) / CLOCKS_PER_NS;
}

double asm_hot_start() {
   int t1, t2;
   asm("cpuid;"
       "rdtsc;"
       "mov %%eax, %0;"
       "mov %%edx, %1;"
      :"=r"(t1), "=r"(t2)
     ::"%eax", "%ebx", "%ecx", "%edx");
   asm("cpuid;"
       "rdtsc;"
       "mov %%eax, %0;"
       "mov %%edx, %1;"
      :"=r"(t1), "=r"(t2)
     ::"%eax", "%ebx", "%ecx", "%edx");
   asm("cpuid;"
       "rdtsc;"
       "mov %%eax, %0;"
       "mov %%edx, %1;"
      :"=r"(t1), "=r"(t2)
     ::"%rax", "%rbx", "%rcx", "%rdx");
   return (double)((((unsigned long)t2) << 32) + ((unsigned long)(t1) & 0xFFFFFFFF)) / CLOCKS_PER_NS;
}

double asm_hot_stop() {
   int t1, t2;
   asm("rdtscp;"
       "mov %%eax, %0;"
       "mov %%edx, %1;"
       "cpuid;"
      :"=r"(t1), "=r"(t2)
     ::"%rax", "%rbx", "%rcx", "%rdx");
   return (double)((((unsigned long)t2) << 32) + ((unsigned long)(t1) & 0xFFFFFFFF)) / CLOCKS_PER_NS;
}

// The monotonic timer in NS, to compete with the asm strategies (cold cache).
double mono_cold_start() {
   struct timespec t1;
   clock_gettime(CLOCK_MONOTONIC, &t1);
   return (double)(t1.tv_sec * 1000000000 + t1.tv_nsec);
}

// The monotonic timer in NS, to compete with the asm strategies (cold cache).
double mono_cold_stop() {
   struct timespec t1;
   clock_gettime(CLOCK_MONOTONIC, &t1);
   return (double)(t1.tv_sec * 1000000000 + t1.tv_nsec);
}

// The monotonic timer in NS, to compete with the asm strategies (cold cache).
double mono_hot_start() {
   struct timespec t1;
   clock_gettime(CLOCK_MONOTONIC, &t1);
   clock_gettime(CLOCK_MONOTONIC, &t1);
   clock_gettime(CLOCK_MONOTONIC, &t1);
   return (double)(t1.tv_sec * 1000000000 + t1.tv_nsec);
}

// The monotonic timer in NS, to compete with the asm strategies (cold cache).
double mono_hot_stop() {
   struct timespec t1;
   clock_gettime(CLOCK_MONOTONIC, &t1);
   return (double)(t1.tv_sec * 1000000000 + t1.tv_nsec);
}
