#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<signal.h>
#include "timers.h"
#include "arrays.h"
#include "output.h"

#define RUN_COUNT 10
#define DELAY_COUNT 100000
double* g_cur_result;
int parent_pid;

void child_handler(int sig, siginfo_t* info, void* arg) {
   kill(parent_pid, SIGUSR1);
}

void parent_handler(int sig, siginfo_t* info, void* arg) {
   (*g_cur_result) = TIMER_STOP;
}

int main(int argc, char** argv) {
   volatile int i, fork_res;
   double result[RUN_COUNT];
   parent_pid = getpid();
   fork_res = fork();

   if (fork_res == 0) {
      struct sigaction sa;
      sa.sa_sigaction = &child_handler;

      sigaction(SIGUSR1, &sa, NULL);

      while(1) {
         pause();
      }
      printf("Error: child exited while loop!\n");
   }



   for (i = 0; i < DELAY_COUNT; i++);
   struct sigaction sa;
   sa.sa_sigaction = &parent_handler;
   sigaction(SIGUSR1, &sa, NULL);
   g_cur_result = result;
   for (i = 0; i < RUN_COUNT; i++) {
      double start = TIMER_START;
      kill(fork_res, SIGUSR1);
      pause();
      result[i]-= start; // Replace with actual value later.
      g_cur_result++;
   }

   double res_min = array_min(result, RUN_COUNT);
   printf("Sigtest min: %lf\n", res_min);
   printf("Sigtest # of mins: %d", occur_of(result, RUN_COUNT, res_min));

   array_to_csv(result, RUN_COUNT, "sigtest.csv");

   return 0;
}
