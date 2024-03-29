#include "client.h"
#include "dbhandle.h"
#include "list.h"
#include "menuutil.h"
#include <assert.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOTRACE

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
 *@param list List to isert data into.
 *@param argc How many columns are there.
 *@param argv Values in columns.
 *@param colNames names of columns.
 *@return 0.
 **/
static int clientGetListQueryCallback(struct List *list, int argc,
                                      const char **argv,
                                      const char **const colNames) {
  assert(list && argv && argc && colNames);
  // printf("list size before push is %d\n", listSize(list));
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

/**
 *@brief Generates SQL query.
 *@param sType ClientSort based on which to sort.
 *@param desc Whather soring should be descending.
 *@return query.
 * */
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
    orderStr = "phoneNumber";
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
 *@param sType Sort type coresponding to @link ClientSort @endlink.
 *@param desc Wheather sorting should be descending.
 *- flase -- ascending
 *- true -- descending
 *@return List of clients. See also Client.
 **/
struct List *clientGetList(int sType, bool desc) {
  struct List *res = NULL;
  char *q = clientGetQueryOfSort(sType, desc);
  dbHandleGetResultAsList(
      &res, (int (*)(void *, int, char **, char **))clientGetListQueryCallback,
      q);
  return res;
}

/**
 *@brief make Clone of Client. It allocates memory internaly.
 *@param dest Client structure where data will be cloned into.
 *@param src Client to create clone of.
 **/
void clientClone(struct Client **dest, const struct Client *src) {
  struct Client *res = NULL;

  res = clientNew();
  res->m_ID = src->m_ID;

  res->m_phoneNum = src->m_phoneNum;

  res->m_cardID = src->m_cardID;

  res->m_adress = calloc(FORMFIELDLENGTH + 1, sizeof(char));
  strcpy(res->m_adress, src->m_adress);

  res->m_name = calloc(FORMFIELDLENGTH + 1, sizeof(char));
  strcpy(res->m_name, src->m_name);

  res->m_surname = calloc(FORMFIELDLENGTH + 1, sizeof(char));
  strcpy(res->m_surname, src->m_surname);

  *dest = res;
}
