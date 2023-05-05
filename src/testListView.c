#include "list.h"
#include "mmenu/menuutil.h"
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
  for (int i = 0; i < 10; ++i) {
    int *t = calloc(sizeof(int), 1);
    *t = i + 1;
    listInsert(list, t, intLess);
  }
  return list;
}

char *getIntString(void *x) {
  char *result = calloc(sizeof(char), FORMFIELDLENGTH + 1);
  sprintf(result, "%*d", FORMFIELDLENGTH, *(int *)x);
  return result;
}

void intDel(void *data) { free(data); }

int main() {
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);

  start_color();
  init_pair(1, COLOR_BLACK, COLOR_GREEN); // debugging color

  const int colCount = 1;
  struct List *(**listGetters)() = calloc(sizeof(void *), colCount);
  listGetters[0] = getList;
  const char *const colNames[] = {"Fuck C"};
  // listViewInvoke(0, 0, listGetters, colNames, colCount, getIntString,
  // intDel);
  listViewInvoke(0, 0, listGetters, colNames, colCount, getIntString, intDel);
  free(listGetters);
  endwin();
}
