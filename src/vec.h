#pragma once

#define DEFAULT_LIST_SIZE 512
#define LIST_GROWTH_FACTOR 2

struct vec {
  int *data;
  size_t count;
  size_t size;
};

typedef struct vec vec_t;

void vec_resize(vec_t *vec);
void vec_add(vec_t *vec, int x);
void vec_clear(vec_t *vec);
