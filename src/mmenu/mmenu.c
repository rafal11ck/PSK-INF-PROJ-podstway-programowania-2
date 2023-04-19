#include "mmenu.h"
#include "carsmenu.h"
#include "clientsmenu.h"
#include "menuutil.h"
#include "rentalsmenu.h"
#include <ncurses.h>
#include <stdlib.h>

/**
 *@file
 *@brief Menu implementation.
 **/

/**
 * @brief Handles main menu.
 */
void mainMenuSelection(void);

/**
 * @brief Starts ncurses
 **/

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
