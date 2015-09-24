#include"timers.h"

#define RUN_COUNT 100000

int main(int argc, char** argv) {
   int i, wait_stat;
   for (i = 0; i < RUN_COUNT; i++) {

      int fork_res = fork();

      // Check if the fork worked.
      if (fork_res < 0) {
         printf("ERROR: fork failed!\n");
         exit(1);
      } else if (fork_res != 0) {

         // This is the parent, it will simply wait
         wait(&wait_stat);
      } else {

         // This is the child. It call getuid to make sure the library is not
         // being brought in cold, then times a call to getpgid.
         getuid();
         pid_t pid = getpid();
         double start = TIMER_START;
         getpgid(pid);
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
