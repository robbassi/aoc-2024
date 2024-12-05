#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define DEFAULT_LIST_SIZE 512
#define LIST_GROWTH_FACTOR 2

struct vec {
  int *data;
  size_t count;
  size_t size;
};

typedef struct vec vec_t;

void vec_resize(vec_t *vec) {
  switch (vec->size) {
    case 0:
      vec->size = DEFAULT_LIST_SIZE;
      vec->data = malloc(vec->size * sizeof(int));
      break;
    default:
      vec->size = vec->size * LIST_GROWTH_FACTOR;
      vec->data = realloc(vec->data, vec->size * sizeof(int));
      break;
  }
}

void vec_add(vec_t *vec, int x) {
  if (vec->count == vec->size) {
    vec_resize(vec);
  }
  vec->data[vec->count++] = x;
}

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
