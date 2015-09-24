#include"timers.h"

#define RUN_COUNT 10000

int main(int argc, char** argv) {
   int i, wait_stat;
   for (i = 0; i < RUN_COUNT; i++) {

      int fork_res = fork();
      
      // Check for a failed fork.
      if (fork_res < 0) {
         printf("ERROR: Failed fork!\n");
         exit(0);
      } else if (fork_res != 0) {
         wait(&wait_stat);
      } else {

         // Have the child access the library first with getuid, then time a
         // call to getpid.
         getuid();
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
