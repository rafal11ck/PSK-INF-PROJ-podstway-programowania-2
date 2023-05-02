#include "stdbool.h"

//! @todo move function documentation into .c file.

/**
 * @file
 * @brief Doubly linked list interface.
 *  */

/**
 * @brief Doubly linked list node.
 * */
struct ListNode {
  //! Pointer to data.
  void *m_data;
  //! Pointer to the previous element of the list.
  //! - NULL if it's first element.
  struct ListNode *m_prev;
  //! Pointer to the next element of the list.
  //! - NULL if it's last element.
  struct ListNode *m_next;
};

/**
 * @brief Stores pointers to first and last elements of double linked list.
 */
struct List {
  //! Pointer to first elemenet of the list.
  struct ListNode *m_front;

  //! Pointer to last elemenet of the list.
  struct ListNode *m_back;

  //! Number of elements in the list.
  int m_size;
};

struct List *listCreateList();

bool listPushFront(struct List *list, void *data);

bool listPushBack(struct List *list, void *data);

bool listInsert(struct List *list, void *data,
                bool (*prevFun)(const void *, const void *));

struct ListNode *listGetFront(struct List *list);

struct ListNode *listGetBack(struct List *list);

bool listDeleteNode(struct List *list, struct ListNode *node);

int listSize(const struct List *const list);
