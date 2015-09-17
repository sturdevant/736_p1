#include<stdio.h>
#include<stdlib.h>

void array_to_csv(long* arr, int size, char* filename) {
   FILE* fd = fopen(filename, "w");
   int i;
   for (i = 0; i < size - 1; i++) {
      fprintf(fd, "%ld,", arr[i]);
   }
   fprintf(fd, "%ld", arr[size-1]);
   fclose(fd);
}
