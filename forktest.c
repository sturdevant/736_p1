#include"timers.h"

#define RUN_COUNT 10000

/*
 * We will fork off processes one at a time and have each print their results
 * to the console. This will be piped to an output file. Experimentation showed
 * that allocating arrays on either the heap or stack caused an increase in
 * execution time relative to the number of pages used and opening another file
 * for output caused a flat increase in execution time.
 */
int main(int argc, char** argv) {
   int i, wait_stat, fork_res;

   // Begin making processes and timing them
   for (i = 0; i < RUN_COUNT; i++) {
      double start = TIMER_START;

      fork_res = fork();

      // Insure that fork really executed and have the child exit.
      if (fork_res == -1) {
         printf("Error, fork failed!\n");
         exit(1);
      } else if (fork_res != 0) {
         wait(&wait_stat);
      } else {
         exit(0);
      }
      double stop = TIMER_STOP;

      // Print out the resul of this fork from the parent.
      if (i == 0) {
         printf("%lf", stop - start);
      } else {
         printf(",%lf", stop - start);
      }

      // Flush the buffer so that new children have no output buffered.
      fflush(stdout);
   }
   return 0;
}
