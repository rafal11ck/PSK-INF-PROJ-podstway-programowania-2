#include "list.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

struct List *listCreateList() {
  struct List *list = malloc(sizeof(struct List));
  // ensure that memory was allocated
  assert(list);
  list->m_front = NULL;
  list->m_back = NULL;
  return list;
};

bool listPushFront(struct List *list, void *data) {
  // create new node
  struct ListNode *node = malloc(sizeof(struct ListNode));
  assert(node);
  node->m_data = data;
  node->m_next = list->m_front;
  node->m_prev = NULL;
  list->m_front = node; // Make new node first in the list.
  if (list->m_back == NULL)
    list->m_back = node;
  return false;
}
