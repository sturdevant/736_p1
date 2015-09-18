#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<signal.h>
#include "timers.h"
#include "arrays.h"
#include "output.h"

#define RUN_COUNT 10

double* g_cur_result;

void child_handler(int sig, siginfo_t* info, void* arg) {
   printf("Child handler\n");
   kill(info->si_pid, SIGUSR1);
}

void parent_handler(int sig, siginfo_t* info, void* arg) {
   printf("Parent handler\n");
   (*g_cur_result) = TIMER_STOP;
}

int main(int argc, char** argv) {
   int i, fork_res;
   double result[RUN_COUNT];

   fork_res = fork();
   printf("fork_res = %d\n", fork_res);
   fflush(stdout);
   if (fork_res == 0) {
      printf("Configuring child thread\n");
      struct sigaction sa;
      sa.sa_sigaction = &child_handler;

      printf("Child setting sig action\n");
      sigaction(SIGUSR1, &sa, NULL);

      printf("Child sig action set\n");
      while(1) {
         printf("Child pausing\n");
         pause();
      }
      printf("Error: child exited while loop!\n");
   }


   struct sigaction sa;
   sa.sa_sigaction = &parent_handler;
   sigaction(SIGUSR1, &sa, NULL);
   g_cur_result = result;
   pause();
   for (i = 0; i < RUN_COUNT; i++) {
      double start = TIMER_START;
      kill(fork_res, SIGUSR1);
      printf("Parent signal sent, going to pause\n");
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
