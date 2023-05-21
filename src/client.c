#include "client.h"
#include "clientsmenu.h"
#include "dbhandle.h"
#include "list.h"
#include "menuutil.h"
#include <assert.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *@file
 *@brief Implements function to operate on client.
 **/

/**
 *@brief Allocates and returns client containing nothing.
 *@return Client object.
 *
 *C style strings are NULL pointers.
 *Client::m_ID is INVALIDCLIENTID
 * */
struct Client *clientNew() {
  struct Client *result = calloc(sizeof(struct Client), 1);
  result->m_ID = INVALIDCLIENTID;
  result->m_cardID = INVALIDCLIENTCARDID;
  result->m_phoneNum = INVALIDCLIENTPHONENUM;
  return result;
}

/**
 *@brief
 *Dealloactes client.
 *@param client
 *Client to be freed.
 **/
void clientFree(struct Client *client) {
  free(client->m_name);
  free(client->m_surname);
  free(client->m_adress);
  free(client);
}

/**
 *@brief Checks if every field despite Client::m_ID in Client is set.
 *@param client Client to check.
 *@return
 *- True if Client is complete.
 *- False otherwise.
 **/
bool clientIsComplete(const struct Client *const client) {
  bool result = true;
  if (!client || client->m_adress == NULL ||
      client->m_cardID == INVALIDCLIENTCARDID || client->m_name == NULL ||
      client->m_surname == NULL || client->m_adress == NULL ||
      client->m_phoneNum == INVALIDCLIENTPHONENUM)
    result = false;
  return result;
}

/**
 *@brief CallBack function for @link dbHandleGetResultAsList @endlink .
 *Transforms row into Client.
 **/
static int clientGetListQueryCallback(struct List *list, int argc,
                                      const char **argv,
                                      const char **const colNames) {
  assert(list && argv && argc && colNames);
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

char *clientGetQueryOfSort(int sType, bool desc) {
  char *query = calloc(1000, sizeof(char));
  strcpy(query,
         "SELECT ID, cardID, name, surname, adress, phoneNumber FROM clients "
         "ORDER BY ");
  assert(sType >= 0 && sType < clientSort_MAX);
  char *orderStr = NULL;
  switch (sType) {
  case clientSort_name:
    orderStr = "name";
    break;
  case clientSort_cardId:
    orderStr = "cardID";
    break;
  case clientSort_surname:
    orderStr = "surname";
    break;
  case clientSort_adress:
    orderStr = "adress";
    break;
  case clientSort_phoneNum:
    orderStr = "cardID";
    break;
  };
  strcat(query, orderStr);
  if (desc)
    strcat(query, " DESC");
  strcat(query, ";");
  return query;
}

/**
 *@brief Get list of clients.
 **/
struct List *clientGetList(int sType, bool desc) {
  struct List *res = NULL;
  char *q = clientGetQueryOfSort(clientSort_cardId, 0);
  dbHandleGetResultAsList(
      &res, (int (*)(void *, int, char **, char **))clientGetListQueryCallback,
      q);
  return res;
}
