#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include "timers.h"
#include "arrays.h"
#include "output.h"

//#define RUN_COUNT 1000000
#define RUN_COUNT 10

int main(int argc, char** argv) {
   int i, fork_res, wait_stat;
   double result;
   open_file("forktest_app.csv");
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
         /* These uncommented make the loop run RUN_COUNT times, i is always 0 
         printf("i: %d ",i);
         printf("pid: %d ",getpid());
         */

         /* These uncommented will cause termination after one loop
         printf("i: %d\n",i);
         printf("pid: %d\n", getpid());
         */

         /* This is maybe the weirdest, a mix between the two causes the one 
          * with the newline to execute once, and the one without to execute
          * RUN_COUNT times
         printf("i: %d\n",i);
         printf("pid: %d ",getpid());
         */
         array_to_opened_csv(&result, 1, "forktest_app.csv");
      } else {
         append_opened_csv(result, "forktest_app.csv");
      }
   }
   close_file();
   return 0;
}
