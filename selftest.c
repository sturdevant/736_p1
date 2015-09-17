#include<stdio.h>
#include<time.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>
#include "timers.h"
#include "arrays.h"
#include "output.h"

#define RUN_COUNT 10000

void* time_self(void* arg) {
   int res;
   double* res_ptr = (double*)arg;

   double start = TIMER_START;
   res = pthread_self();
   double stop = TIMER_STOP;

   (*res_ptr) = stop - start;
   return NULL;
}

int main(int argc, const char* argv[]) {
   double many_results[RUN_COUNT];
   double single_results[RUN_COUNT];   
   int i;
   
   /* An array of pthread, giving them each a unique ID, shouldn't be able to
      cache */
   printf("Many Threads At A Time: \n");
   pthread_t p[RUN_COUNT];
   for (i = 0; i < RUN_COUNT; i++) {
      pthread_create(&p[i], NULL, &time_self, &many_results[i]);
   }

   for (i = 0; i < RUN_COUNT; i++) {
      pthread_join(p[i], NULL);
   }

   array_to_csv(many_results, RUN_COUNT, "many_self.csv");
   double res_min = array_min(many_results, RUN_COUNT);
   printf("Min = %lf\nMin count = %d\n", res_min, occur_of(many_results, RUN_COUNT, res_min));
   
   /* Alternatively, we can try this, although I was a bit hesitant given that
    * they each get the same thread ID...*/

   printf("One Thread At A Time: \n");
   pthread_t p1;

   for (i = 0; i < RUN_COUNT; i++) {
      pthread_create(&p1, NULL, &time_self, (void*)&single_results[i]);
      pthread_join(p1, NULL);
   }

   array_to_csv(single_results, RUN_COUNT, "single_self.csv");
   res_min = array_min(single_results, RUN_COUNT);
   printf("Min = %lf\nMin count = %d\n", res_min, occur_of(single_results, RUN_COUNT, res_min));
   

   return(0);
}
