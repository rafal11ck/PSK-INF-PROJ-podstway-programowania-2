#include "list.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * @file
 * @brief Doubly linked list implementation.
 * */

/**
 * @brief Instantiates ListNode containing pointer to data.
 * @param data Pointer to data.
 * @return Pointer to ListNode that was instatntitated.
 * */
struct ListNode *listCreateNode(void *data);

/**
 * @brief Inserts data into list before given node.
 * @param list List pointer.
 * @param node Node before which data should be inserted.
 * @param data Data to be inserted pointer.
 * @return False if everything is fine.
 * List has to be non empty;
 * */
bool listInsertBefore(struct List *list, struct ListNode *node, void *data);

struct List *listCreateList() {
  struct List *list = malloc(sizeof(struct List));
  // ensure that memory was allocated
  assert(list);
  list->m_front = NULL;
  list->m_back = NULL;
  return list;
};

bool listPushFront(struct List *list, void *data) {
  struct ListNode *node = listCreateNode(data);
  // if List is empty
  if (list->m_back == NULL) {
    list->m_back = node;
  } else {
    // Make old first node previous of new first node.
    node->m_next = list->m_front;
    node->m_next->m_prev = node;
  }
  list->m_front = node;
  return false;
}

bool listPushBack(struct List *list, void *data) {

  struct ListNode *node = listCreateNode(data);
  // if List is empty
  if (list->m_front == NULL) {
    list->m_front = node;
  } else {
    // Make old last node previous of new last node.
    node->m_prev = list->m_back;
    node->m_prev->m_next = node;
  }
  list->m_back = node;
  return false;
}

bool listInsert(struct List *list, void *data,
                bool (*prevFun)(const void *, const void *)) {
  struct ListNode *it = list->m_front;
  // go to next element as long as cureent element has to be after new element
  while (it != NULL && prevFun(it->m_data, data)) {
    it = it->m_next;
  }
  // if first is what we look for or list is empty
  if (it == list->m_front)
    listPushFront(list, data);
  else if (it == NULL)
    listPushBack(list, data);
  else {
    listInsertBefore(list, it, data);
  }
  return true;
}

struct ListNode *listCreateNode(void *data) {
  struct ListNode *node = malloc(sizeof(struct ListNode));
  assert(node);
  node->m_data = data;
  node->m_next = NULL;
  node->m_prev = NULL;
  return node;
}

bool listInsertBefore(struct List *list, struct ListNode *node, void *data) {
  // Ensure list is non empty.
  assert(list->m_front != NULL);
  struct ListNode *newNode = listCreateNode(data);
  // Make newNode previous point to node that is previous to the node that we
  // insert berfore.
  newNode->m_prev = node->m_prev;
  // Make node that we insert before next of newNode.
  newNode->m_next = node;

  // Make node preceeding newNode point to newNode.
  newNode->m_prev->m_next = newNode;
  // Make node after newNode point to newNode.
  node->m_prev = newNode;
  return false;
}
