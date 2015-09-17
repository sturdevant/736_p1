
// Returns the maximum of two values
double max(double v1, double v2) {
   double m = (v1 > v2) ? v1 : v2;
   return m;
}

// Returns the maximum of an array
double array_max(double* values, int size) {
   int i;
   double m = values[0];
   for (i = 0; i < size; i++)
      m = max(m, values[i]);
   return m;
}

// Returns the minimum of two values
double min(double v1, double v2) {
   double m = (v1 < v2) ? v1 : v2;
   return m;
}

// Returns the minimum of an array
double array_min(double* values, int size) {
   int i;
   double m = values[0];
   for (i = 0; i < size; i++)
      m = min(m, values[i]);
   return m;
}

// Returns first index of value in array, -1 if not in array
int index_of(double* values, int size, double value) {
   int i;
   for (i = 0; i < size; i++)
      if (values[i] == value)
         return i;
   
   return -1;
}

// Returns the number of occurances of value in array
int occur_of(double* values, int size, double value) {
   int i = index_of(values, size, value);
   if (i < 0)
      return 0;
   return 1 + occur_of(&values[i+1], size - (i + 1), value);
}

// Returns the maximum gap between two occurances of a value in the array
int max_gap(double* values, int size, double value) {
   int g = -1;
   int n = occur_of(values, size, value);
   int i, j;
   j = 0;
   for (i = 0; i < n; i++) {
      int c = index_of(&values[j], size - j, value);
      g = max(g, c);
      j += c + 1;
   }
   return max(g, size - j);
}
