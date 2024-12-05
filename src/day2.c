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

bool is_safe_report(vec_t *report, bool use_dampener, int skip) {
  assert(report->count > 1);
  assert((int)report->count > skip);
  int i = 0;
  if (i == skip) i++;
  int j = i + 1;
  if (j == skip) j++;
  bool isIncreasing = report->data[i] < report->data[j];
  while (j < report->count) {
    int curr = report->data[i];
    int next = report->data[j];
    int diff = abs(curr - next);
    if (curr < next != isIncreasing || diff < 1 || diff > 3) {
      if (use_dampener) {
        return is_safe_report(report, false, i - 1)
          || is_safe_report(report, false, i)
          || is_safe_report(report, false, j);
      }
      return false;
    }
    i++;
    if (i == skip) i++;
    j = i + 1;
    if (j == skip) j++;
  }
  return true;
}

int main(int arc, char **argv) {
  vec_t report = {0};
  int safe_reports_p1 = 0;
  int safe_reports_p2 = 0;

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
      if (is_safe_report(&report, false, -1)) {
        safe_reports_p1++;
      }
      if (is_safe_report(&report, true, -1)) {
        safe_reports_p2++;
      }
      vec_clear(&report);
    }
  }

  // Output.
  printf("%d\n", safe_reports_p1);
  printf("%d\n", safe_reports_p2);
  return EXIT_SUCCESS;
}
