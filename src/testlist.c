#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @file
 * @brief List unit test.
 * */

//! Number of items to push during tests
#define N 5

//! Compare 2 ints
bool intCmp(const void *a, const void *b) { return *(int *)(a) < *(int *)(b); }

/**
 * @brief Runs unit test of list.
 * */
int main() {
  printf("Create Empty list...");
  struct List *list = NULL;
  list = listCreateList();
  puts("PASS");

  printf("Add elements the front of list...");
  for (int i = 1; i < N + 1; ++i) {
    int *data = malloc(sizeof(int));
    *data = i;
    listPushFront(list, data);
  }
  puts("PASS");

  printf("Add elements the back of list...");
  for (int i = 20; i < N + 21; ++i) {
    int *data = malloc(sizeof(int));
    *data = i;
    listPushBack(list, data);
  }
  puts("PASS");

  printf("Insert element at front of list...");
  listInsert(list, list->m_front, &intCmp);
  puts("PASS");
}
