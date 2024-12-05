#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define DEFAULT_LIST_SIZE 1024
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

// Clears the vector, but retains any allocated memory for reuse.
void vec_clear(vec_t *vec) {
  vec->count = 0;
}

bool is_safe_report(vec_t *report) {
  assert(report->count > 1);
  bool isIncreasing = report->data[0] < report->data[1];
  for (int i = 1; i < report->count; i++) {
    int last = report->data[i - 1];
    int curr = report->data[i];
    int diff = abs(last - curr);
    if (last < curr != isIncreasing || diff < 1 || diff > 3) {
      return false;
    }
  }
  return true;
}

int main(int arc, char **argv) {
  vec_t report = {0};
  int safe_reports = 0;

  // Parse the reports.
  char buff[1024];
  int x, i = 0;
  for (char c; (c = getchar()) != EOF;) {
    // Collect numeric chars.
    if (c >= '0' && c <= '9') {
      buff[i++] = c;
    } 

    // Parse collected chars, add to report.
    else {
      buff[i] = '\0';
      i = 0;
      sscanf(buff, "%d", &x);
      vec_add(&report, x);
    }

    // Check if the report is safe, and clear the report.
    if (c == '\n') {
      if (is_safe_report(&report)) {
        safe_reports++;
      }
      vec_clear(&report);
    }
  }

  // Output.
  printf("%d", safe_reports);
  return EXIT_SUCCESS;
}
