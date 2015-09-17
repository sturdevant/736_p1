#include<stdio.h>
#include<stdlib.h>

void array_to_csv(double* arr, int size, char* filename) {
   FILE* fd = fopen(filename, "w");
   int i;
   for (i = 0; i < size - 1; i++) {
      fprintf(fd, "%lf,", arr[i]);
   }
   fprintf(fd, "%lf", arr[size-1]);
   fclose(fd);
}

void append_csv(double value, char* filename) {
   FILE* fd = fopen(filename, "w");
   fprintf(fd, ",%lf", value);
   fclose(fd);
}
