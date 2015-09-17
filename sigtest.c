#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<signal.h>
#include "timers.h"
#include "arrays.h"
#include "output.h"

#define RUN_COUNT 10000

long* g_cur_result;

void child_handler(int sig, siginfo_t* info, void* arg) {
   kill(info->si_pid, SIGUSR1);
}

void parent_handler(int sig, signifo_t* info, void* arg) {
   volatile int 
}

int main(int argc, char** argv) {
   int i, fork_res, wait_stat;
   long result[RUN_COUNT];

   fork_res = fork();
   if (fork_res == 0) {
      struct sigaction sa;
      sa.sa_sigaction = &child_handler;
      sigaction(SIGUSR1, sa, NULL);
      while(1) {
         pause();
      }
      printf("Error: child exited while loop!\n");
   }


   struct sigaction sa;
   sa.sa_sigaction = &parent_handler;
   sigaction(SIGUSR1, sa, NULL);
   g_cur_result = result;
   for (i = 0; i < RUN_COUNT; i++) {
      g_cur_result++;
      TIMER_START
      kill();
      pause();
      result[i]-= 1; // Replace with actual value later.
   }

   long res_min = array_min(result, RUN_COUNT);
   printf("Sigtest min: %ld\n", res_min);
   printf("Sigtest # of mins: %d", occur_of(result, RUN_COUNT, res_min));

   array_to_csv(result, RUN_COUNT, "sigtest.csv");

   return 0;
}