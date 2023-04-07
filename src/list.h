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
 * @brief Stores pointers to first and last elements of double linked list.
 */
struct List {
  //! Pointer to first elemenet of the list.
  struct ListNode *m_front;

  //! Pointer to last elemenet of the list.
  struct ListNode *m_back;
};

/**
 * @brief Returns empty list
 * @return Poitner to empty list.
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
 */
bool listPushBack(struct List *list, void *data);

/**
 * @brief Inserts data in list at appropriate positon so that list remains
 * sorted.
 * @param list List into which item is inserted.
 * @param data Pointer to data that will be inserted.
 * @param prevFun Pointer to function that compares two data instances.
 * @return false if everything is fine.
 */
bool listInsert(struct List *list, void *data,
                bool (*prevFun)(const void *, const void *));

/**
 * @brief Returns pointer to the first element of list.
 * @param list List pointer of which first element is wanted.
 * @return Pointer to the first element in the List.
 * - Returns NULL if List is empty.
 */
struct ListNode *listGetFront(struct List *list);

/**
 * @brief Returns pointer to the last element of list.
 * @param list List pointer of which last element is wanted.
 * @return Pointer to the last element in the List.
 * - Returns NULL if List is empty.
 */
struct ListNode *listGetBack(struct List *list);

/**
 * @brief Deallocates memory block pointed by  ListNode::m_data and removes
 * ListNode from List.
 * @param list List from which ListNode has to be deleted.
 * @param node ListNode for removal.
 * @return False if deleted successfully.
 */
bool listDeleteNode(struct List *list, struct ListNode *node);
