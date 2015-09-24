#include"timers.h"

#define RUN_COUNT 250000
#define LOOP_SIZE_CHANGE 10
#define MIN_LOOP_SIZE 100
#define TIMER_COUNT 2

// Define a global variable to track the number of loops to iterate over.
volatile int g_n_loops;

// The execution time of this function will be timed.
inline void timed_func() {
   volatile int var;
   for (var = 0; var < g_n_loops + MIN_LOOP_SIZE; var++)
      ; // Do nothing.
}

// Forward declarations for each of the timer functions.
double asm_hot();
double mono_hot();

/*
 * The driver of this test. Defines an array of timers and runs each with the
 * function to be timed.
 */
int main(int argc, char** argv) {
	
   double* t_result = malloc(sizeof(double) * TIMER_COUNT * LOOP_SIZE_CHANGE);
   double (*timer_func[TIMER_COUNT])(void);
   timer_func[0] = &asm_hot;
   timer_func[1] = &mono_hot;
   int i, j;
   double cur_min[TIMER_COUNT];


   for (i = 0; i < TIMER_COUNT; i++) {
      cur_min[i] = INT_MAX;
   }

   // Set the number of loops being timed to 0 initially.
   g_n_loops = 0;
   for (i = 0; i < RUN_COUNT; i++) {

      // Record the minimum for the last loop size and increment.
      if (i % (RUN_COUNT / LOOP_SIZE_CHANGE) == 0 && i > 0) {
         for (j = 0; j < TIMER_COUNT; j++) {
            t_result[j * LOOP_SIZE_CHANGE + g_n_loops] = cur_min[j];
            cur_min[j] = INT_MAX;
         }
         g_n_loops++;
      }

      for (j = 0; j < TIMER_COUNT; j++) {
         double new_time = timer_func[j]();
         if (new_time < cur_min[j]) {
            cur_min[j] = new_time;
         }
      }
   }

   for (j = 0; j < TIMER_COUNT; j++) {
      t_result[j * LOOP_SIZE_CHANGE + g_n_loops] = cur_min[j];
      cur_min[j] = INT_MAX;
   }
 
   array_to_csv(t_result, LOOP_SIZE_CHANGE, "asm_hot.csv");
   array_to_csv(&t_result[LOOP_SIZE_CHANGE], LOOP_SIZE_CHANGE, "mono_hot.csv");

   for (i = 0; i < TIMER_COUNT; i++) {
      printf("Min #%d = %lf\n", i, array_min(&t_result[i * LOOP_SIZE_CHANGE], LOOP_SIZE_CHANGE));
   }
   return 0;
}

// The suggested Intel timer with proper serialization and a warm cache
double asm_hot() {
   double start = asm_hot_start();
   timed_func();
   double stop = asm_hot_stop();
   return stop - start;
}

// The monotonic timer in ns, to compete with the asm strategies (warm cache).
double mono_hot() {
   double start = mono_hot_start();
   timed_func();
   double stop = mono_hot_stop();
   return stop - start;
}
