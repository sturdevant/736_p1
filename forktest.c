#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include "timers.h"
#include "arrays.h"
#include "output.h"

#define RUN_COUNT 200000

int main(int argc, char** argv) {
   int i, fork_res, wait_stat;
   long result[RUN_COUNT];

   for (i = 0; i < RUN_COUNT; i++) {
      TIMER_START
      fork_res = fork();
      if (fork_res != 0) {
         wait(&wait_stat);
      } else {
         exit(0);
      }
      TIMER_STOP
      result[i] = timer_result;
   }

   long res_min = array_min(result, RUN_COUNT);
   printf("Forktest min: %ld\n", res_min);
   printf("Forktest # of mins: %d", occur_of(result, RUN_COUNT, res_min));

   array_to_csv(result, RUN_COUNT, "forktest.csv");

   return 0;
}
