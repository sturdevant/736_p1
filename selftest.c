#include "timers.h"

#define RUN_COUNT 10000

void* time_self(void* arg) {
   int res;
   double* res_ptr = (double*)arg;

   // Call pthread_equal in case the library needs to be brought in, so that
   // time wont be added to the subsequent call of pthread_self.
   pthread_equal(0, 0);

   // Run the call to time.
   double start = TIMER_START;
   res = pthread_self();
   double stop = TIMER_STOP;

   (*res_ptr) = stop - start;
   return NULL;
}

int main(int argc, const char* argv[]) {
   double results[RUN_COUNT];   
   int i;
   pthread_t p1;

   // Create threads one at a time and have each time pthread_self.
   for (i = 0; i < RUN_COUNT; i++) {
      pthread_create(&p1, NULL, &time_self, (void*)&single_results[i]);
      pthread_join(p1, NULL);
   }

   array_to_csv(results, RUN_COUNT, "self.csv");
   res_min = array_min(results, RUN_COUNT);
   printf("Min = %lf\nMin count = %d\n", res_min, occur_of(results, RUN_COUNT, res_min));
   

   return(0);
}
