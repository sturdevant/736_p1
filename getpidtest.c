#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include "timers.h"
#include "arrays.h"
#include "output.h"

#define RUN_COUNT 100000

int main(int argc, char** argv) {
   int i, wait_stat;
   for (i = 0; i < RUN_COUNT; i++) {

      if (fork() != 0) {
         wait(&wait_stat);
      } else {
         double start = TIMER_START;
         getpid();
         double stop = TIMER_STOP;
         if (i == 0) {
            printf("%lf", stop - start);
         } else {
            printf(",%lf", stop - start);
         }
         fflush(stdout);
         exit(0);
      }
   }
   return 0;
}
