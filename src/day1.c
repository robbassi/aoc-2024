#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "vec.h"

int compare_int(const void *a, const void *b) {
  return *(int*)a - *(int*)b;
}

int main(int argc, char **argv) {
  vec_t vec_a = {0};
  vec_t vec_b = {0};

  // Build the lists.
  int a, b;
  while (scanf("%d   %d", &a, &b) != EOF) {
    vec_add(&vec_a, a);
    vec_add(&vec_b, b);
  }
  assert(vec_a.count == vec_b.count);

  // Sort the data.
  qsort(vec_a.data, vec_a.count, sizeof(int), compare_int);
  qsort(vec_b.data, vec_b.count, sizeof(int), compare_int);

  // Compute the distance.
  int total_distance = 0;
  for (int i = 0; i < vec_a.count; i++) {
    total_distance += abs(vec_a.data[i] - vec_b.data[i]);
  }

  // Compute the similarity score.
  int similarity_score = 0;
  int freq, j = 0, last_a = -1;
  for (int i = 0; i < vec_a.count; i++) {
    int a = vec_a.data[i];
    if (a != last_a) {
      for (freq = 0; j < vec_b.count; j++) {
        int b = vec_b.data[j];
        if (b < a) continue;
        else if (b == a) freq++;
        else if (b > a) break;
      }
    }
    similarity_score += a * freq;
    last_a = a;
  }

  // Output.
  printf("%d\n", total_distance);
  printf("%d\n", similarity_score);
  return EXIT_SUCCESS;
}
