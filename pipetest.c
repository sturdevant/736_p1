#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<signal.h>
#include "timers.h"
#include "arrays.h"
#include "output.h"

#define RUN_COUNT 200000

/*
 * Creates a child process and communicates with RUN_COUNT exchanges across two
 * pipes. Each value is timed and the minimum is printed, along with its
 * frequency.
 */
int main(int argc, char** argv) {
   int i, fork_res;

   // Keep a large result array in the heap.
   double* result = malloc(RUN_COUNT * sizeof(double));

   // A pipe written by the parent, read by the child (parent to child).
   int p2c_pipe[2];

   // A pipe written by the child, read by the parent (child to parent).
   int c2p_pipe[2];

   // This one character buffer will repeatedly be sent across the pipe.
   char buf = 'a';

   // Create the pipes and validate their creation.
   if (pipe(p2c_pipe) == -1 || pipe(c2p_pipe) == -1) {
      printf("Error: pipe creation failure!\n");
      return -1;
   }

   // Fork off a child process and check for an error.
   fork_res = fork();
   if (fork_res == -1) {
      printf("Error: fork failure!\n");
      return -1;
   } else if (fork_res == 0) {

      // CHILD CODE //
      
      // Close the unused ends of each pipe.
      close(p2c_pipe[1]);
      close(c2p_pipe[0]);

      // Indefinitely respond to messages via the c2p pipe.
      while(1) {
         read(p2c_pipe[0], &buf, 1);
         write(c2p_pipe[1], &buf, 1);
      }

      // We should never get here in the child
      printf("Error: child exited while loop!\n");

      // END OF CHILD CODE //
   }

   // PARENT EXECUTES HERE //

   // Close the unused ends of each pipe.
   close(p2c_pipe[0]);
   close(c2p_pipe[1]);

   // Keep writing and waiting for a response and record each time.
   for (i = 0; i < RUN_COUNT; i++) {
      double start = TIMER_START;
      write(p2c_pipe[1], &buf, 1);
      read(c2p_pipe[0], &buf, 1);
      double stop = TIMER_STOP;
      result[i] = stop - start;
   }

   // Print some basic output to the console.
   double res_min = array_min(result, RUN_COUNT);
   printf("Pipetest min: %lf, %lf\n", 
          res_min, 
          (double)occur_of(result, RUN_COUNT, res_min) / RUN_COUNT);

   // Print the full result out to a csv for plotting.
   array_to_csv(result, RUN_COUNT, "pipetest.csv");

   // Kill the child and return
   kill(fork_res, SIGKILL);
   return 0;
}
