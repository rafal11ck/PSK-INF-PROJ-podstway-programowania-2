#include "carsmenu.h"
#include "menuutil.h"
#include <stdlib.h>

/**
 *@file
 *@brief Cars menu implementation
 **/

void carsMenu(void) {
  const char *const title = "Cars";
  const char *const choices[] = {
      "listCars",   "searchCars", "addCars",
      "removeCars", "editCars",   "returnToMainMenu",
  };
  const int choicesCount = sizeof(choices) / sizeof(choices[0]);
  //! @todo impelemnt submenus
  void (*menuFun[])(void) = {NULL, NULL, NULL, NULL, NULL, NULL};
  invokeMenu(title, choices, choicesCount, menuFun);
}
