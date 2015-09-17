#include<stdio.h>

#include<time.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>
#include"output.h"
#include"arrays.h"
#include"timers.h"

#define RUN_COUNT 250000
#define MAX_LOOP_SIZE 100
#define TIMER_COUNT 6

#define OPERATIONS_TO_TIME \
int lv; \
volatile int x; \
for (lv = 0; lv < g_n_loops; lv++) \
   x = 0;

volatile int g_n_loops;

// Forward declarations for each of the timer functions.
double asm_base();
double asm_base_for();
double asm_cold();
double asm_hot();
double mono_cold();
double mono_hot();

int main(int argc, char** argv) {
	
   double t_result[TIMER_COUNT][RUN_COUNT];
   double (*timer_func[TIMER_COUNT])(void);
   timer_func[0] = &asm_base;
   timer_func[1] = &asm_base_for;
   timer_func[2] = &asm_cold;
   timer_func[3] = &asm_hot;
   timer_func[4] = &mono_cold;
   timer_func[5] = &mono_hot;
   int i, j;

   g_n_loops = 0;
   for (i = 0; i < RUN_COUNT; i++) {


      // Increment up the number of loops being timed.
      if (i % (RUN_COUNT / MAX_LOOP_SIZE) == 0 && i > 0) {
         g_n_loops++;
         printf("Min result for %d = %lf\n", g_n_loops, array_min(&t_result[0][i - RUN_COUNT/MAX_LOOP_SIZE + 1], RUN_COUNT/MAX_LOOP_SIZE - 1));
      }

      for (j = 0; j < TIMER_COUNT; j++) {
         t_result[j][i] = timer_func[j]();
      }
   }

   array_to_csv(t_result[0], RUN_COUNT, "asm_base.csv");
   array_to_csv(t_result[1], RUN_COUNT, "asm_base_for.csv");
   array_to_csv(t_result[2], RUN_COUNT, "asm_cold.csv");
   array_to_csv(t_result[3], RUN_COUNT, "asm_hot.csv");
   array_to_csv(t_result[4], RUN_COUNT, "mono_cold.csv");
   array_to_csv(t_result[5], RUN_COUNT, "mono_hot.csv");

   for (i = 0; i < TIMER_COUNT; i++) {
      printf("Min #%d = %lf\n", i, array_min(&t_result[i][0], RUN_COUNT));
   }
   return 0;
}

// Our original timer function without proper serialization
double asm_base() {
   double start = asm_hot_start();
   OPERATIONS_TO_TIME
   double stop = asm_hot_stop();
   return stop - start;
}

// Our original timer with a trailing for loop to show unordered execution
double asm_base_for() {
   double start = asm_base_for_start();
   OPERATIONS_TO_TIME
   double stop = asm_base_for_stop();
   volatile int i;
   volatile int j;
   for (i = 0; i < 5; i++) {
      j = i;
   }
   return stop - start;
}

// The suggested Intel timer with proper serialization and a cold cache
double asm_cold() {
   double start = asm_cold_start();
   OPERATIONS_TO_TIME
   double stop = asm_cold_stop();
   return stop - start;
}

// The suggested Intel timer with proper serialization and a warm cache
double asm_hot() {
   double start = asm_hot_start();
   OPERATIONS_TO_TIME
   double stop = asm_hot_stop();
   return stop - start;
}

// The monotonic timer in NS, to compete with the asm strategies (cold cache).
double mono_cold() {
   double start = mono_cold_start();
   OPERATIONS_TO_TIME
   double stop = mono_cold_stop();
   return stop - start;
}

// The monotonic timer in NS, to compete with the asm strategies (warm cache).
double mono_hot() {
   double start = mono_hot_start();
   OPERATIONS_TO_TIME
   double stop = mono_hot_stop();
   return stop - start;
}
