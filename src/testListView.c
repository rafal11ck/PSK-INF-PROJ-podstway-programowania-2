#include "list.h"
#include "menuutil.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool intLess(const void *a, const void *b) {
  return *(int *)(a) < *(int *)(b) ? true : false;
}

struct List *getList(void) {
  // create list and insert elements.
  struct List *list = listCreateList();
  for (int i = 0; i < 100; ++i) {
    int *t = calloc(sizeof(int), 1);
    *t = i + 1;
    listInsert(list, t, intLess);
  }
  return list;
}

struct List *listGetter(int sType, bool desc) {
  return getList();
}

char *getIntString(void *x) {
  char *result = calloc(sizeof(char), 2 * FORMFIELDLENGTH + 1);
  sprintf(result, "%*d%*d", FORMFIELDLENGTH, *(int *)x, FORMFIELDLENGTH, 1);
  return result;
}

void extract(void **out, const struct ListNode *const data) {
  int *result = calloc(1, sizeof(int));
  *result = *(int *)data->m_data;
  *out = result;
}

void intDel(void *data) { free(data); }

void randomListExample(void) {
  const char *const colNames[] = {"C is bad", "Lua is better"};
  const int colCount = sizeof(colNames) / sizeof(*colNames);
  int *out = NULL;
  // invoke choice
  bool didChose = listViewInvoke((void **)&out, extract, listGetter, colNames,
                                 colCount, getIntString, intDel);

  if (didChose) {
    printf("Value of chosen element = %d", *out);
  } else {
    printf("Canceled.");
  }

  free(out);
}

int main() {
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  curs_set(0);

  start_color();
  init_pair(1, COLOR_BLACK, COLOR_GREEN); // debugging color
  randomListExample();
  endwin();
}
