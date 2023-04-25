#include "clientsmenu.h"
#include "../client.h" //! @todo remove that dots but LSP is retarded and complains, despite cmake working...
#include "menuutil.h"
#include "mmenu.h"
#include <assert.h>
#include <form.h>
#include <ncurses.h>
#include <panel.h>
#include <stdio.h>
#include <stdlib.h>

/**
 *@file
 *@brief Clients menu implementation.
 */

/**
 * @brief function for editing client.
 * @return NULL if client was not edited, pointer to client if was edited.
 *
 * @todo given client pointer make form have initial values.
 */
struct Client *editClientForm() {
  const char *const formFieldNames[] = {"Card id", "Name", "Surname", "Address",
                                        "Phone Number"};
  int fieldCount = sizeof(formFieldNames) / sizeof(*formFieldNames);

  FORM *form = formInit(fieldCount);
  set_field_type(form_fields(form)[0], TYPE_INTEGER, 0, 0, 0);
  formInvoke(form, formFieldNames, "Client");
  formFree(form);
  return NULL;
}

/**
 *@todo Implement
 *@brief function for adding client;
 **/
void addClient(void) {
  const char *const title = "Add client";
  const char *const fieldNames[] = {"cos1", "cos2", "cos3"};
  const int fieldCount = sizeof(fieldNames) / sizeof(*fieldNames);

  struct Client *newClient = editClientForm();
}

/**
 *@brief Handles displaying of clients menu.
 */
void clientsMenu(void) {

  const char *const title = "Clients";
  const char *const choices[] = {"listClients", "Add client", "removeClients",
                                 "editClients", "Return to main menu"};
  const int choicesCount = sizeof(choices) / sizeof(choices[0]);
  //! @todo implement submenus
  void (*menuFun[])(void) = {NULL, addClient, NULL, NULL, NULL};
  menuInvoke(title, choices, choicesCount, menuFun);
}
