#include "arrays.h"
#include "output.h"
#include<stdio.h> 
#include<stdlib.h> 

int main(int argc, char * argv[]) {
   long arr[100];
   int i;
   for (i = 0; i < 100; i++) 
      arr[i] = i + 20;
   
   arr[30] = 22;
   arr[31] = 22;
   arr[41] = 22;
   arr[61] = 22;
   arr[71] = 22;
   arr[81] = 22;
   arr[91] = 22;
   printf("Minimum: %ld\nMaximum: %ld\n",array_min(arr,100), array_max(arr, 100));
   printf("First index of 50: %d\n", index_of(arr, 100, 50));
   printf("Occurances of 50: %d\n", occur_of(arr, 100, 50));
   printf("First index of 22: %d\n", index_of(arr, 100, 22));
   printf("Occurances of 22: %d\n", occur_of(arr, 100, 22));
   printf("Max gap of 22: %d\n", max_gap(arr, 100, 22));
   array_to_csv(arr, 100, "out.csv");
}
