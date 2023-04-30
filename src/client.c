#include "client.h"
#include <stdlib.h>

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
