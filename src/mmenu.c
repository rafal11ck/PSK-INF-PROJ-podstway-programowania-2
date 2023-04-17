#include "mmenu.h"
#include "dbhandle.h"
#include <menu.h>
#include <ncurses.h>
#include <panel.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *@file
 *@brief Menu implementation.
 **/

#define MENUMARK (" * ")
// #define _NDEBUG

/**
 * @brief Handles main menu.
 */
void mainMenuSelection(void);

/**
 * @brief Handles cars menu.
 * */
void carsMenu(void);

/**
 * @brief Handles clients menu.
 * */
void clientsMenu(void);

/**
 * @brief Handles rentals menu.
 * */
void rentalsMenu(void);

void mainMenu(void) {
  if (dbHandleOpenDB())
    fprintf(stderr, "Database error exiting.\n");
  else {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    mainMenuSelection();
    endwin();
  }
}

void mainMenuSelection(void) {
  char *title = "Main menu";
  char *opcje[] = {"Cars", "Clients", "Rentals", "Exit"};
  const int liczbaOpcji = sizeof(opcje) / sizeof(opcje[0]);
  void (*menuFun[liczbaOpcji])(void);
  menuFun[0] = carsMenu;
  menuFun[1] = clientsMenu;
  menuFun[2] = rentalsMenu;
  menuFun[3] = NULL;

  ITEM **mainMenuItems = calloc(liczbaOpcji + 1, sizeof(ITEM *));
  for (int i = 0; i < liczbaOpcji; ++i) {
    mainMenuItems[i] = new_item(opcje[i], opcje[i]);
    set_item_userptr(mainMenuItems[i], menuFun[i]);
  }
  mainMenuItems[liczbaOpcji] = NULL;

  // boarders + title + options
  const int windowRows = liczbaOpcji + 4;

  // assume title is longest (don't count mark)
  long unsigned longestChoice = strlen(title) - strlen(MENUMARK);
  for (int i = 0; i < liczbaOpcji; ++i) {
    const int temp = strlen(opcje[i]);
    if (temp > longestChoice)
      longestChoice = temp;
  }

  // +2 for boarders, +2 because menu leaves 2 empty columns
  int windowCols = strlen(MENUMARK) + longestChoice + 2 + 2;
  // make title centered
  if ((windowCols ^ strlen(title) & 1))
    ++windowCols;

  WINDOW *mainMenuWindow =
      newwin(windowRows, windowCols, (LINES - windowRows) / 2,
             (COLS - windowCols) / 2);

  PANEL *panel = new_panel(mainMenuWindow);

  MENU *mainMenu = new_menu(mainMenuItems);
  set_menu_win(mainMenu, mainMenuWindow);
  keypad(mainMenuWindow, TRUE);
  // substract boarders and title
  set_menu_sub(mainMenu,
               derwin(mainMenuWindow, liczbaOpcji, windowCols - 4, 3, 1));

  set_menu_mark(mainMenu, MENUMARK);
  box(mainMenuWindow, 0, 0);

  mvwprintw(mainMenuWindow, 1, (getmaxx(mainMenuWindow) - (strlen(title))) / 2,
            "%s", title);
  mvwaddch(mainMenuWindow, 2, 0, ACS_LTEE);
  mvwhline(mainMenuWindow, 2, 1, ACS_HLINE, getmaxx(mainMenuWindow) - 2);
  mvwaddch(mainMenuWindow, 2, getmaxx(mainMenuWindow) - 1, ACS_RTEE);

  set_menu_items(mainMenu, mainMenuItems);
  menu_opts_off(mainMenu, O_SHOWDESC);

  post_menu(mainMenu);

  int input;
  bool doExit = FALSE;
  while (!doExit) {
    update_panels();
    doupdate();
    input = getch();
    switch (input) {
    case KEY_UP:
      menu_driver(mainMenu, REQ_UP_ITEM);
      break;
    case KEY_DOWN:
      menu_driver(mainMenu, REQ_DOWN_ITEM);
      break;
    case 10:;
      ITEM *curitem = current_item(mainMenu);
      const char *const name = item_name(curitem);
      hide_panel(panel);
#ifndef _NDEBUG
      move(1, 1);
      clrtoeol();
      mvprintw(1, 1, "MAIN MENU SELECTED: %s", name);
#endif
      // EXIT has nullpointer, break the swich and loop
      if (item_userptr(curitem) == NULL) {
        doExit = TRUE;
        break;
      }
      ((void (*)(void))(item_userptr(curitem)))();
      show_panel(panel);
    }
  }
  unpost_menu(mainMenu);
  free_menu(mainMenu);
  for (int i = 0; i < liczbaOpcji; ++i)
    free_item(mainMenuItems[i]);
}

void carsMenu() {
  char *opcje[] = {"listCars",   "searchCars", "addCars",
                   "removeCars", "editCars",   "returnToMainMenu"};
  const int liczbaOpcji = sizeof(opcje) / sizeof(opcje[0]);

  int currentChoice = 0;

  int choice;
  while (true) {
    clear();
    refresh();
    printw("Cars Menu:\n");

    // wyswietl opcje
    for (int i = 0; i < liczbaOpcji; i++) {
      // print arrow for current option
      if (i == currentChoice)
        printw(" -> ");
      else
        printw("    ");
      printw("%s\n", opcje[i]);
    }

    switch (choice = getch()) {
    case KEY_UP:
      if (currentChoice > 0)
        --currentChoice;
      break;
    case KEY_DOWN:
      if (currentChoice < liczbaOpcji - 1)
        ++currentChoice;
      break;
    case 10:
      switch (currentChoice) {
      case 0:
        printw("Wybrales opcje: listCars\n");
        break;
      case 1:
        printw("Wybrales opcje: searchCars\n");
        break;
      case 2:
        printw("Wybrales opcje: addCars\n");
        break;
      case 3:
        printw("Wybrales opcje: removeCars\n");
        break;
      case 4:
        printw("Wybrales opcje: editCars\n");
        break;
      case 5:
        return;
      }
      refresh();
      getch();
    }
  }
}

void clientsMenu() {
  char *opcje[] = {"listClients", "addClients", "removeClients", "editClients",
                   "returnToMainMenu"};
  int liczbaOpcji = sizeof(opcje) / sizeof(opcje[0]);

  int currentChoice = 0;
  while (true) {
    clear();
    refresh();
    printw("Clients Menu:\n");

    for (int i = 0; i < liczbaOpcji; i++) {
      // print arrow for current option
      if (i == currentChoice)
        printw(" -> ");
      else
        printw("    ");
      printw("%s\n", opcje[i]);
    }

    refresh();

    int choice;
    switch (choice = getch()) {
    case KEY_UP:
      if (currentChoice > 0)
        --currentChoice;
      break;
    case KEY_DOWN:
      if (currentChoice < liczbaOpcji - 1)
        ++currentChoice;
      break;
    case 10:
      switch (currentChoice) {
      case 0:
        printw("Wybrales opcje: listClients\n");
        break;
      case 1:
        printw("Wybrales opcje: addClients\n");
        break;
      case 2:
        printw("Wybrales opcje: removeClients\n");
        break;
      case 3:
        printw("Wybrales opcje: editClients\n");
        break;
      case 4:
        return;
      }
    }
  }
}

void rentalsMenu() {
  char *opcje[] = {"listRents", "addRent", "returnRent", "returnToMainMenu"};
  int liczbaOpcji = sizeof(opcje) / sizeof(opcje[0]);

  int currentChoice = 0;
  while (true) {
    clear();
    refresh();
    printw("Rents Menu:\n");

    for (int i = 0; i < liczbaOpcji; i++) {
      // print arrow for current option
      if (i == currentChoice)
        printw(" -> ");
      else
        printw("    ");
      printw("%s\n", opcje[i]);
    }

    refresh();
    int choice;
    switch (choice = getch()) {
    case KEY_UP:
      if (currentChoice > 0)
        --currentChoice;
      break;
    case KEY_DOWN:
      if (currentChoice < liczbaOpcji - 1)
        ++currentChoice;
      break;
    case 10:
      switch (currentChoice) {
      case 0:
        printw("Wybrales opcje: listRents\n");
        break;
      case 1:
        printw("Wybrales opcje: addRent\n");
        break;
      case 2:
        printw("Wybrales opcje: returnRent\n");
        break;
      case 3:
        return;
      }
    }
  }
}
