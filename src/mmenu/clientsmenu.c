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

  // allocate
  FIELD *field[fieldCount + 1];
  field[fieldCount - 1] = NULL;
  for (int i = 0; i < fieldCount - 1; ++i) {
    field[i] = new_field(1, 10, 2 * i, 1, 0, 0);
    assert(field[i]);
    set_field_back(field[i], A_UNDERLINE);
  }

  // make form go on screen
  FORM *form = new_form(field);
  post_form(form);

  //! @todo parse form

  // free memory
  unpost_form(form);
  free_form(form);
  for (int i = 0; i < fieldCount - 1; ++i) {
    free_field(field[i]);
  }

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
  invokeMenu(title, choices, choicesCount, menuFun);
}
