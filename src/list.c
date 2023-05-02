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

/**
 * @brief Free memory pointed by ListNode::m_data and ListNode.
 * @param node For removal.
 * @return False if succeed.
 */
bool listDealocateListNode(struct ListNode *node) {
  assert(node != NULL);
  free(node->m_data);
  free(node);
  return false;
}

/**
 * @brief Returns empty list
 * @return Poitner to empty list.
 */
struct List *listCreateList() {
  struct List *list = malloc(sizeof(struct List));
  // ensure that memory was allocated
  assert(list);
  list->m_front = NULL;
  list->m_back = NULL;
  list->m_size = 0;
  return list;
};

/**
 * @brief Adds item at the front of list.
 * @param list List into which item is added.
 * @param data Pointer to data that will be pushed.
 * @return false if everything is fine.
 * */
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
  ++list->m_size;
  return false;
}

/**
 * @brief Adds item at the end of list.
 * @param list List into which item is added.
 * @param data Pointer to data that will be pushed.
 * @return false if everything is fine.
 */
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
  ++list->m_size;
  return false;
}

/**
 * @brief Inserts data in list at appropriate positon so that list remains
 * sorted.
 * @param list List into which item is inserted.
 * @param data Pointer to data that will be inserted.
 * @param prevFun Pointer to function that compares two data instances.
 * @return false if everything is fine.
 */
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
  ++list->m_size;
  return false;
}

/**
 * @brief Returns pointer to the first element of list.
 * @param list List pointer of which first element is wanted.
 * @return Pointer to the first element in the List.
 * - Returns NULL if List is empty.
 */
struct ListNode *listGetFront(struct List *list) {
  return list->m_front;
}

/**
 * @brief Returns pointer to the last element of list.
 * @param list List pointer of which last element is wanted.
 * @return Pointer to the last element in the List.
 * - Returns NULL if List is empty.
 */
struct ListNode *listGetBack(struct List *list) {
  return list->m_back;
}

/**
 * @brief Deallocates memory block pointed by  ListNode::m_data and removes
 * ListNode from List.
 * @param list List from which ListNode has to be deleted.
 * @param node ListNode for removal.
 * @return False if deleted successfully.
 */
bool listDeleteNode(struct List *list, struct ListNode *node) {
  assert(node != NULL);
  // If node for removal is first in the list
  if (node == listGetFront(list)) {
    list->m_front = node->m_next;
    // If it's only one ListNode in List.
    if (node == listGetBack(list))
      list->m_back = node->m_prev;
    else
      node->m_next->m_prev = node->m_prev;
  }
  // ListNode for removal is not at front in the List.
  else {
    node->m_prev->m_next = node->m_next;
    // node for removal is last in the list
    if (node == listGetBack(list))
      list->m_back = node->m_prev;
    else
      node->m_next->m_prev = node->m_prev;
  }
  --list->m_size;
  listDealocateListNode(node);
  return false;
}

/**
 * @brief How many elements are there in List.
 * @param list List of which size is to be retrived.
 * @return Size of list.
 * */
int listSize(const struct List *const list) { return list->m_size; }
