#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include "timers.h"
#include "arrays.h"
#include "output.h"

#define RUN_COUNT 1000000

int main(int argc, char** argv) {
   int i, fork_res, wait_stat;
   double result[RUN_COUNT];

   for (i = 0; i < RUN_COUNT; i++) {
      double start = TIMER_START;
      fork_res = fork();
      if (fork_res != 0) {
         wait(&wait_stat);
      } else {
         exit(0);
      }
      double stop = TIMER_STOP;
      result[i] = stop - start;
   }

   double res_min = array_min(result, RUN_COUNT);
   printf("Forktest min: %lf\n", res_min);
   printf("Forktest # of mins: %d", occur_of(result, RUN_COUNT, res_min));

   array_to_csv(result, RUN_COUNT, "forktest.csv");

   return 0;
}
