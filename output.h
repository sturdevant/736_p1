#include<stdio.h>
#include<stdlib.h>

// Current file we have opened (global var, so be cautious!)
FILE* curr;

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

void open_file(char* filename) {
   curr = fopen(filename, "w");
}

void close_file(){
   fclose(curr);
}

void array_to_opened_csv(double* arr, int size, char* filename) {
   int i;
   for (i = 0; i < size - 1; i++)
      fprintf(curr, "%lf,", arr[i]);
   fprintf(curr, "%lf",arr[size-1]);
}

void append_opened_csv(double value, char* filename) {
   fprintf(curr, ",%lf", value);
}
