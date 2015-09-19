#include<stdio.h>
#include<time.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>
#include"timers.h"
#include"arrays.h"
#include"output.h"

#define RUN_COUNT 10

pthread_mutex_t lock1;
pthread_mutex_t lock2;

void* lockandblock() {
   pthread_mutex_lock(&lock1);
   pthread_mutex_unlock(&lock2);
   return NULL;
}

int main(int argc, char** argv) {
   double results[RUN_COUNT];
   int i;

   pthread_t p;

   for (i = 0; i < RUN_COUNT; i++) {
      pthread_mutex_lock(&lock1);
      pthread_create(&p, NULL, &lockandblock, NULL);
      double start = TIMER_START;
      pthread_mutex_unlock(&lock1);
      pthread_mutex_lock(&lock2);
      double stop = TIMER_STOP;
      results[i] = stop - start;

      pthread_join(p, NULL);
      pthread_mutex_unlock(&lock1);
      pthread_mutex_unlock(&lock2);
   }

   array_to_csv(results, RUN_COUNT, "lockandblock.csv");
   return(0);
}
