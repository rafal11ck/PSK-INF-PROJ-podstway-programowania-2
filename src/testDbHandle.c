#include "client.h"
#include "clientsmenu.h"
#include "dbhandle.h"
#include "list.h"
#include "menuutil.h"
#include "time.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cb(void *pt, int argc, char **argv, char **colnames) {
  for (int i = 0; i < argc; ++i) {
    printf("%30s,%50s\n", colnames[i], argv[i]);
    printf("\n");
  }
  return 0;
}

int cb2(void *pt, int argc, char **argv, char **colnames) {
  printf("%d\n", listSize(pt));
  struct Client *c = clientNew();

  listPushBack(pt, c);
  return 0;
}

static int cb3(void *list, int argc, char **argv, char **const colNames) {
  printf("list size before push is %d\n", listSize(list));
  struct Client *cl = clientNew();
  for (int i = 0; i < argc; ++i) {
    const char *colName = colNames[i];
    const char *val = argv[i];
    if (!strcmp(colName, "ID")) {
      cl->m_ID = atoi(val);
    } else if (!strcmp(colName, "cardID")) {
      cl->m_cardID = atoi(val);
    } else if (!strcmp(colName, "name")) {
      cl->m_name = calloc(FORMFIELDLENGTH + 1, sizeof(char));
      strcpy(cl->m_name, val);
    } else if (!strcmp(colName, "surname")) {
      cl->m_surname = calloc(FORMFIELDLENGTH + 1, sizeof(char));
      strcpy(cl->m_surname, val);
    } else if (!strcmp(colName, "phoneNumber")) {
      cl->m_phoneNum = atoi(val);
    } else if (!strcmp(colName, "adress")) {
      cl->m_adress = calloc(FORMFIELDLENGTH + 1, sizeof(char));
      strcpy(cl->m_adress, val);
    } else {
      fprintf(stderr, "Client to structure fail. FAILED on %s", colName);
      abort();
    }
  }
  listPushBack(list, cl);
  return 0;
}

int main() {

  srand(time(0));
  struct List *res = NULL;
  char *q = clientGetQueryOfSort(rand() % clientSort_MAX, 0);
  dbHandleGetResultAsList(&res, cb3, q);
  printf("Query =\t %s\n", q);
  printf("Result list size  = %d\n", listSize(res));
  struct ListNode *it = listGetFront(res);

  while (it != NULL) {
    struct Client *c = it->m_data;
    char *str = clientGetListViewString(c);
    printf("%s\n", str);
    it = it->m_next;
  }

  // not freed.
}
