#include "mmenu.h"
#include "carsmenu.h"    //For submenu.
#include "clientsmenu.h" //For submenu.
#include "menuutil.h"    //For displaying
#include "rentalsmenu.h" //For submenu.
#include <ncurses.h>     //For displaying.
#include <stdlib.h>      //For NULL.

/**
 *@file
 *@brief Menu implementation.
 **/

/**
 * @brief Handles main menu.
 */
void mainMenuSelection(void);

void mainMenuSelection(void) {
  const char *const title = "Main menu";
  const char *const choices[] = {"Cars", "Clients", "Rentals", "Exit"};
  const int choicesCount = sizeof(choices) / sizeof(choices[0]);
  void (*menuFun[])(void) = {carsMenu, clientsMenu, rentalsMenu, NULL};
  invokeMenu(title, choices, choicesCount, menuFun);
}

void mainMenu(void) {
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  mainMenuSelection();
  endwin();
}
