#include "rentalsmenu.h"
#include "menuutil.h"
#include <stdlib.h>

/**
 *@file
 *@brief Rentals menu implementation
 **/

void rentalsMenu(void) {
  const char *const title = "Rentals";
  const char *const choices[] = {"listRents", "addRent", "returnRent",
                                 "returnToMainMenu"};
  const int choicesCount = sizeof(choices) / sizeof(choices[0]);
  //! \todo implemnet submenus
  void (*menuFun[])(void) = {NULL, NULL, NULL, NULL};
  invokeMenu(title, choices, choicesCount, menuFun);
}
