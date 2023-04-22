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
 *@brief Allocates form
 *@param formFieldNames Array of field names.
 *@param fieldCount size of array.
 *@return form.
 *with fieldCount fields, based of field names array
 *
 *@todo Support for validation options.
 * */
FORM *makeForm(const int fieldCount) {
  assert(fieldCount > 0);
  // One more as last one is sentinel.
  FIELD *field[fieldCount + 1];
  field[fieldCount] = NULL;
  // -1 as last is sentinel
  for (int i = 0; i < fieldCount - 1; ++i) {
    // 3 rows from top, 2 boarders and title;
    // columns offset is boarder + field names
    //! @todo offesets
    field[i] = new_field(1, FORMFIELDLENGTH, 1, 1, 0, 0);
    set_field_back(field[i], A_UNDERLINE);
  }
  FORM *form = new_form(field);

  //! @todo THIS MIGHT BE CAUSING PROBLEMS BUT RESOLVED MEMORY LEAK.
  //!  so needs to be checked.
  for (int i = 0; i < fieldCount - 1; ++i) {
    free_field(field[i]);
  }
  return form;
}

/**
 *@brief Deallocate form.
 **/
void freeForm(FORM *form) {

  int fieldCount = field_count(form);
  assert(fieldCount == ERR || fieldCount <= 0);
  unpost_form(form);
  for (int i = 0; i < fieldCount; ++i)
    free_field(form_fields(form)[i]);
  free_form(form);
}

/**
 * @brief function for editing client.
 * @return NULL if client was not edited, pointer to client if was edited.
 *
 * @todo given client pointer make form have initial values.
 */
struct Client *editClientForm() {
  const char *const formFieldNames[] = {"Card id", "Name", "Surname", "Address",
                                        "Phone Number"};
  int fieldCount = sizeof(formFieldNames) / sizeof(formFieldNames[0]);

  FORM *form = makeForm(fieldCount);
  //! @todo post it on screen for input.
  invokeForm(form, "Client", formFieldNames);
  //! @todo free memory.
  freeForm(form);
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
