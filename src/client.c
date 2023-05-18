#include "client.h"
#include <assert.h>
#include <stdbool.h>
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
 *C style strings are NULL.
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
 *@
 **/
struct List *clientGetList(int sType, bool desc) {
  char *query = calloc(500, sizeof(char));
  query = "SELECT ID, cardID, name, surname, adress, phoneNumber FROM cleints "
          "ORDER BY";
  assert(sType > 0 && sType < clientSort_MAX);
  char *orderStr = calloc(100, sizeof(char));
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
    orderStr = "";
    break;
  case clientSort_phoneNum:
    orderStr = "cardID";
    break;
  };
  free(orderStr);
  if (desc)
    strcat(query, "DESC");
  strcat(query, ";");
  free(query);
}
