#include"timers.h"

#define RUN_COUNT 10000

// A trivial child thread which does nothing but exit.
void* exit_self(void* arg) {
   pthread_exit(NULL);
   return NULL;
}

int main(int argc, const char* argv[]) {
   double results[RUN_COUNT];
   int i;
   
   /* Alternatively, we can try this, although I was a bit hesitant given that
    * they each get the same thread ID...*/

   pthread_t p1;

   for (i = 0; i < RUN_COUNT; i++) {

      // Time the creation and joining with a thread.
      double start = TIMER_START;
      pthread_create(&p1, NULL, &exit_self, NULL);
      pthread_join(p1, NULL);
      double stop = TIMER_STOP;
      results[i] = stop - start;
   }

   array_to_csv(results, RUN_COUNT, "threadcreate.csv");
   double res_min = array_min(results, RUN_COUNT);
   printf("Min = %lf\nMin count = %d\n", res_min, occur_of(results, RUN_COUNT, res_min));
   

   return(0);
}
