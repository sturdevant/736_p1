#include"timers.h"

#define RUN_COUNT 10000000

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cv2 = PTHREAD_COND_INITIALIZER;

pthread_mutex_t ready_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ready_cv = PTHREAD_COND_INITIALIZER;

int child_ready, child_done, parent_done;

void* waitandsignal() {
   // Have the child grab this lock to be used in signalling.
   pthread_mutex_lock(&lock1);

   // Indicate in shared memory that we are ready.
   child_ready = 1;

   // Signal the sleeping main thread.
   pthread_cond_signal(&cv1);
   while(1) {
      // Wait for the thread to respond, also giving up the lock.
      while (!parent_done)
         pthread_cond_wait(&cv1, &lock1);
      parent_done = 0;
      // We've been woken up, so let's lock in order to properly respond and
      // signal followed by an unlock.
      pthread_mutex_lock(&lock2);
      child_done = 1;
      pthread_cond_signal(&cv2);
      pthread_mutex_unlock(&lock2);
   }
   return NULL;
}

int main(int argc, char** argv) {
   double* results = (double*)malloc(sizeof(double) * RUN_COUNT);
   int i;

   pthread_t p;
   child_ready = 0;
   parent_done = 0;
   pthread_mutex_lock(&lock2);
   pthread_mutex_lock(&lock1);
   int success = pthread_create(&p, NULL, &waitandsignal, NULL);
      
   // Check for error
   //pthread_mutex_unlock(&lock2);
   if (success != 0){
      fprintf(stderr,"Thread not created\n");
      exit(0);
   }
   
   // Wait for the child to be ready on the first lock to exchange.
   while (child_ready == 0)
      pthread_cond_wait(&cv1, &lock1);

   for (i = 0; i < RUN_COUNT; i++) {
 
      child_done = 0;
      double start = TIMER_START;
      // Signal the child to run and unlock the lock for it.
      parent_done = 1;
      pthread_cond_signal(&cv1);
      pthread_mutex_unlock(&lock1);

      // Wait for the child to signal on lock 2.
      while (!child_done)
         pthread_cond_wait(&cv2, &lock2);
      double stop = TIMER_STOP;

      results[i] = stop - start;

      pthread_mutex_lock(&lock1);
   }

   array_to_csv(results, RUN_COUNT, "cvtest.csv");
   return(0);
}
