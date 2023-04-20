#include "clientsmenu.h"
#include "menuutil.h"
#include <stdlib.h>

/**
 *@file
 *@brief Clients menu implementation.
 */

/**
 *@brief Handles displaying of clients menu.
 */
void clientsMenu(void) {

  const char *const title = "Clients";
  const char *const choices[] = {"listClients", "addClients", "removeClients",
                                 "editClients", "Return to main menu"};
  const int choicesCount = sizeof(choices) / sizeof(choices[0]);
  //! @todo impelemnt submenus
  void (*menuFun[])(void) = {NULL, NULL, NULL, NULL, NULL};
  invokeMenu(title, choices, choicesCount, menuFun);
}

/**
 *\todo Implement
 *
 *@biref FORM for editing client;
 **/
void editClient() {}
