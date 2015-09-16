
// Returns the maximum of two values
long max(long v1, long v2) {
   long m = (v1 > v2) ? v1 : v2;
   return m;
}

// Returns the maximum of an array
long array_max(long* values, int size) {
   int i;
   long m = values[0];
   for (i = 0; i < size; i++)
      m = max(m, values[i]);
   return m;
}

// Returns the minimum of two values
long min(long v1, long v2) {
   long m = (v1 < v2) ? v1 : v2;
   return m;
}

// Returns the minimum of an array
long array_min(long* values, int size) {
   int i;
   long m = values[0];
   for (i = 0; i < size; i++)
      m = min(m, values[i]);
   return m;
}

// Returns first index of value in array, -1 if not in array
int index_of(long* values, int size, long value) {
   int i;
   for (i = 0; i < size; i++)
      if (values[i] == value)
         return i;
   
   return -1;
}

// Returns the number of occurances of value in array
int occur_of(long* values, int size, long value) {
   int i = index_of(values, size, value);
   if (i < 0)
      return 0;
   return 1 + occur_of(&values[i+1], size - (i + 1), value);
}

// Returns the maximum gap between two occurances of a value in the array
int max_gap(long* values, int size, long value) {
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
