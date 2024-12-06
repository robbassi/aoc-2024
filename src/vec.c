#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "vec.h"

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

// Clears the vector, but retains any allocated memory for reuse.
void vec_clear(vec_t *vec) {
  vec->count = 0;
}

