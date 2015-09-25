#include"timers.h"

#define RUN_COUNT 10000000

// Initialize two locks for our threads to exchange blocking on.
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

// The child thread function, blocks on lock 1 and releases lock two once it has
// gotten lock 1.
void* lockandblock() {
   while(1) {
      pthread_mutex_lock(&lock1);
      pthread_mutex_unlock(&lock2);
   }
   printf("ERROR: child thread exited!\n");
   return NULL;
}

// Our driving function. It will create threads one at a time which immediately
// block on lock 1, waiting for the parent to release it.
int main(int argc, char** argv) {
   double* results = malloc(sizeof(double) * RUN_COUNT);
   int i;

   pthread_t p;

   // Take locks 1 and 2 before creating a child so the child thread will
   // block on lock 1 if it runs first and the parent will block on lock 2
   // if it runs first. Now, a context switch is forced in the timing.
   pthread_mutex_lock(&lock1);
   pthread_mutex_lock(&lock2);
   pthread_create(&p, NULL, &lockandblock, NULL);

   for (i = 0; i < RUN_COUNT; i++) {
      
      // Begin timing and unlock lock 1. The child must be run before lock 2
      // can be unlocked, so this thread will be forced to wait.
      double start = TIMER_START;
      pthread_mutex_unlock(&lock1);

      // It is possible to be descheduled here and have the child run through,
      // with the locks not forcing any context switching; however, being
      // descheduled and rescheduled will not result in the minimum time, so
      // such runs are effectively ignored in our reporting.
      pthread_mutex_lock(&lock2);
      double stop = TIMER_STOP;
      results[i] = stop - start;

   }

   array_to_csv(results, RUN_COUNT, "locktest.csv");
   return(0);
}
