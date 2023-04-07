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
 *@brief Prints gien List to standard output.
 *@param list List to be printed pointer.
 *@returns false
 **/
bool printList(struct List *list) {
  printf("\nlist=\t");
  struct ListNode *it = listGetFront(list);
  while (it != NULL) {
    printf("%d\t", *(int *)(it->m_data));
    it = it->m_next;
  }
  printf("\n");
  return false;
}

/**
 * @brief Runs unit test of list.
 * */
int main() {
  printf("Create Empty list...");
  struct List *list = NULL;
  list = listCreateList();
  printList(list);

  printf("Add elements the front of list...");
  for (int i = 1; i < N + 1; ++i) {
    int *data = malloc(sizeof(int));
    *data = i;
    listPushFront(list, data);
  }
  printList(list);

  printf("Add elements the back of list...");
  for (int i = 20; i < N + 21; ++i) {
    int *data = malloc(sizeof(int));
    *data = i;
    listPushBack(list, data);
  }
  printList(list);

  printf("Insert given node...");
  struct ListNode *it = listGetFront(list);
  int *x = malloc(sizeof(int));
  *x = 3;
  listInsert(list, x, &intCmp);
  printList(list);
}
