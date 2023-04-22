#include "clientsmenu.h"
#include "menuutil.h"
#include "mmenu.h"
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
 *\todo Implement
 *
 *@brief FORM for editing client;
 **/
void addClient() {
  const char *const title = "Add client";
  const char *const fieldNames[] = {"cos1", "cos2", "cos3"};
  const int fieldCount = sizeof(fieldNames) / sizeof(*fieldNames);

  invokeForm(title, fieldNames, fieldCount);
}

/**
 *@brief Handles displaying of clients menu.
 */
void clientsMenu(void) {

  const char *const title = "Clients";
  const char *const choices[] = {"listClients", "Add client", "removeClients",
                                 "editClients", "Return to main menu"};
  const int choicesCount = sizeof(choices) / sizeof(choices[0]);
  //! @todo impelemnt submenus
  void (*menuFun[])(void) = {NULL, addClient, NULL, NULL, NULL};
  invokeMenu(title, choices, choicesCount, menuFun);
}
