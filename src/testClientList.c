#include "client.h"
#include "list.h"
#include "mmenu/clientsmenu.h"
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

int main() { // struct List *list = clientGetList(0, 0);
             //

  printf("Single client test.\n");
  struct Client *c = clientNew();
  clientFree(c);

  printf("clientGetList test\n");
  struct List *clist = clientGetList(0, false);
  printf("\tSize of list = %d\n", listSize(clist));
  /*
  struct ListNode *it = listGetFront(clist);
  while (it != NULL) {
    // clientGetListViewString(it->m_data);
    struct Client *cl = it->m_data;
    printf("%s", cl->m_adress);
    it = it->m_next;
  }
  */
}
