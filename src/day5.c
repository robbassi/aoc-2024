#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "vec.h"

#define MAX_PAGES 100
int before[MAX_PAGES][MAX_PAGES];

int compare_page(const void *p1, const void *p2) {
  int page1 = *(int*)p1;
  int page2 = *(int*)p2;

  if (before[page1][page2]) {
    return -1;
  } else if (before[page2][page1]) {
    return 1;
  }

  return 0;
}

#define BUFF_SIZE 256
void parseRules() {
  int x, y;
  char line[BUFF_SIZE];
  while (fgets(line, BUFF_SIZE, stdin)) {
    if (sscanf(line, "%d|%d", &x, &y) == 2) {
      before[x][y] = 1;
    } else {
      break;
    }
  }
}

// Check if the update is ordered correctly.
bool update_is_valid(vec_t *update, bool fix) {
  for (int i = 0; i < update->count; i++) {
    int page = update->data[i];
    
    for (int j = 0; j < i; j++) {
      int prev_page = update->data[j];
      if (before[page][prev_page]) {
        if (fix) {
          qsort(update->data, update->count, sizeof(int), compare_page);
          return true;
        }
        return false;
      }
    }
  }

  return true;
}

int main(int argc, char **argv) {
  vec_t update = {0};
  int middle_sum_p1 = 0;
  int middle_sum_p2 = 0;

  parseRules();

  // Parse the page updates, update the sums as we go.
  char buff[1024];
  int i = 0;
  int page, last_page = -1;
  for (char c; (c = getchar()) != EOF;) {
    if (c >= '0' && c <= '9') {
      buff[i++] = c;
    } else {
      buff[i] = '\0';
      i = 0;
      sscanf(buff, "%d", &page);
      vec_add(&update, page);
    }

    if (c == '\n') {
      if (update_is_valid(&update, false)) {
        middle_sum_p1 += update.data[update.count / 2];
      } else if (update_is_valid(&update, true)) {
        middle_sum_p2 += update.data[update.count / 2];
      }
      vec_clear(&update);
    }
  }

  printf("%d\n", middle_sum_p1);
  printf("%d\n", middle_sum_p2);
  return EXIT_SUCCESS;
}
