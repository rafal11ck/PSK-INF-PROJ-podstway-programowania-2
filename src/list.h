#include "stdbool.h"

/**
 * @brief Doubly linked list node.
 * */
struct ListNode {
  //! Pointer to data.
  void *m_data;
  //! Pointer to the previous element of the list.
  struct ListNode *m_prev;
  //! Pointer to the next element of the list.
  struct ListNode *m_next;
};

/**
 * @brief Holds list.
 **/
struct List {
  //! Pointer to first elemenet of the list.
  struct ListNode *m_front;

  //! Pointer to last elemenet of the list.\n
  //! - If list is empty holds `NULL`.
  struct ListNode *m_back;
};

/**
 * @return Returns poitner to empty list.
 */
struct List *listCreateList();

/**
 * @brief Adds item to the front of list.
 * @param list List to which item is added.
 * @param data Pointer to data.
 * @return false if everything is fine.
 * */
bool listPushFront(struct List *list, void *data);
