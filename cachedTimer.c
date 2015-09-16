#include<stdio.h>
#include<time.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>

#define RUN_COUNT 100

void* time_self() {
   int t1, t2, t3, t4;
   volatile int x;
   asm("rdtscp; mov %%eax, %0;":"=a"(t1));
   asm(        "mov %%edx, %0;":"=a"(t2));
   x = pthread_self();
   asm("rdtscp; mov %%eax, %0;":"=a"(t3));
   asm(        "mov %%edx, %0;":"=a"(t4));

   return (void *) ((long) ((t4 - t2) * 1000000000 + t3 - t1));
}

int main(int argc, const char* argv[]) {
   long t_result[RUN_COUNT];
   void *result;
   int i;
   
   /* An array of pthread, giving them each a unique ID, shouldn't be able to
      cache */
   printf("Many Threads At A Time: \n");
   pthread_t p[RUN_COUNT];
   for (i = 0; i < RUN_COUNT; i++) {
      pthread_create(&p[i], NULL, &time_self, NULL);
   }

   for (i = 0; i < RUN_COUNT; i++) {
      pthread_join(p[i], &result);
      printf("%ld\n",(long)result);
   }
   
   /* Alternatively, we can try this, although I was a bit hesitant given that
    * they each get the same thread ID...*/

   printf("One Thread At A Time: \n");
   pthread_t p1;

   for (i = 0; i < RUN_COUNT; i++) {
      pthread_create(&p1, NULL, &time_self, NULL);
      pthread_join(p1, &result);
      printf("%ld\n",(long)result);
   }
   return(0);
}
