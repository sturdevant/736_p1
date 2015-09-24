#include"timers.h"

// The number of pages to run the test on. Its size is limited by the huge page
// size of the machine, which is 512 pages. At a number higher than this, the
// processor will bring in 512 pages at once, giving artifically low access
// times because there wont be any faulting for those pages.
#define PAGES 500

// We will allocate a large array of ints, PAGES + 1 of them.
int main(int argc, const char* argv[]) {
   double results[PAGES];
   int i, buf = 1;

   // Determine the page size on the machine. Usually, this is 4kB.
   int psize = getpagesize();
   int* big_array = malloc((PAGES + 1) * psize);

   // Verfiy that our malloc was successful.
   if (big_array == NULL) {
      printf("Error, malloc failed!");
      exit(1);
   }

   // Begin on the second page allocated so we are guarenteed to fault and
   // access pages 1 at a time, recording the time for each access.
   int step = psize / sizeof(int);
   for (i = 0; i < PAGES; i++) {
      double start = TIMER_START;
      big_array[step * (i + 1)] = buf;
      double stop = TIMER_STOP;
      results[i] = stop - start;
   }

   array_to_csv(results, PAGES, "malloc.csv");
   double res_min = array_min(results, PAGES);
   printf("Page size = %d\n", psize);
   printf("Min = %lf\nMin count = %d\n", res_min, occur_of(results, PAGES, res_min));
   

   return(0);
}
