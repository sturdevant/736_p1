#include"timers.h"

#define RUN_COUNT 1000

// The execution time of this function will be timed.
inline void timed_func() {
   volatile int var;
   for (var = 0; var < 5000; var++)
      ; // Do nothing, we just want some base value
}


/*
 * The driver of this test. Defines an array of timers and runs each with the
 * function to be timed.
 */
int main(int argc, char** argv) {
	
   double results[RUN_COUNT];
   int i;

   // Run the clock on the timed function RUN_COUNT times.
   for (i = 0; i < RUN_COUNT; i++) {
         double start = TIMER_START;
         timed_func();
         double stop = TIMER_STOP;
         results[i] = stop - start;
   }
 
   array_to_csv(results, RUN_COUNT, "var.csv");
   return 0;
}
