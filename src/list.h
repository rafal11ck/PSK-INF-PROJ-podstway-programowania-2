#include "stdbool.h"

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
 * @brief Holds list.
 **/
struct List {
  //! Pointer to first elemenet of the list.
  struct ListNode *m_front;

  //! Pointer to last elemenet of the list.
  struct ListNode *m_back;
};

/**
 * @brief Returns empty list
 * @return Returns poitner to empty list.
 */
struct List *listCreateList();

/**
 * @brief Adds item at the front of list.
 * @param list List into which item is added.
 * @param data Pointer to data that will be pushed.
 * @return false if everything is fine.
 * */
bool listPushFront(struct List *list, void *data);

/**
 * @brief Adds item at the end of list.
 * @param list List into which item is added.
 * @param data Pointer to data that will be pushed.
 * @return false if everything is fine.
 * */
bool listPushBack(struct List *list, void *data);

/**
 * @brief Inserts data in list at appropriate positon so that list remains
 * sorted.
 * @param list List into which item is inserted.
 * @param data Pointer to data that will be inserted.
 * @param prevFun Pointer to function that compares two data instances.
 * @return false if everything is fine.
 **/
bool listInsert(struct List *list, void *data,
                    bool (*prevFun)(const void *, const void *));
