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
   double result;

   for (i = 0; i < RUN_COUNT; i++) {
      double start = TIMER_START;
      fork_res = fork();
      if (fork_res != 0) {
         wait(&wait_stat);
      } else {
         exit(0);
      }
      double stop = TIMER_STOP;
      result = stop - start;
      if (i == 0) {
         array_to_csv(&result, 1, "forktest_app.csv");
      } else {
         append_csv(result, "forktest_app.csv");
      }
   }

   return 0;
}
