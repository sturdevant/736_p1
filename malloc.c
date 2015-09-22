#include<stdio.h>
#include<time.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include "timers.h"
#include "arrays.h"
#include "output.h"

#define PAGES 800

void* exit_self(void* arg) {
   pthread_exit(NULL);
   return NULL;
}

int main(int argc, const char* argv[]) {
   double results[PAGES];
   int i;
   char buf;
   int psize = getpagesize();
   char* big_array = malloc((PAGES + 1) * 1 * psize);
   /* Alternatively, we can try this, although I was a bit hesitant given that
    * they each get the same thread ID...*/

   for (i = 0; i < PAGES; i++) {
      double start = TIMER_START;
      big_array[psize * 1 * (i + 1)] = buf;
      double stop = TIMER_STOP;
      results[i] = stop - start;
   }

   array_to_csv(results, PAGES, "malloc.csv");
   double res_min = array_min(results, PAGES);
   printf("Page size = %d\n", psize);
   printf("Min = %lf\nMin count = %d\n", res_min, occur_of(results, PAGES, res_min));
   

   return(0);
}
