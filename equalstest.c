#include "timers.h"

#define RUN_COUNT 10000


/*
 * Our child function. It will write its result out to the location specified
 * by arg. Note: it calls pthread_self first, so pthread_equals has valid args
 * and so it has made a thread call, in case that changes timing.
 */
void* time_self(void* arg) {
   int res;
   double* res_ptr = (double*)arg;
   pthread_t p = pthread_self();
   res = pthread_equal(0, 0);
   double start = TIMER_START;
   res = pthread_equal(p, p);
   double stop = TIMER_STOP;

   (*res_ptr) = stop - start;
   return NULL;
}

/*
 * Creates RUN_COUNT threads, 1 at a time and has each record the time to call
 * pthread_equal.
 */
int main(int argc, const char* argv[]) {
   double results[RUN_COUNT];   
   int i;
   
   /* I was a bit hesitant given that they each get the same thread ID...*/
   pthread_t p1;

   // Create threads one at a time and pass them a pointer to record their
   // results.
   for (i = 0; i < RUN_COUNT; i++) {
      pthread_create(&p1, NULL, &time_self, (void*)&results[i]);
      pthread_join(p1, NULL);
   }

   // Print the results to a csv and the minimum to the console.
   array_to_csv(results, RUN_COUNT, "equals.csv");
   double res_min = array_min(results, RUN_COUNT);
   printf("Min = %lf\nMin count = %d\n", res_min, occur_of(results, RUN_COUNT, res_min));

   return(0);
}
