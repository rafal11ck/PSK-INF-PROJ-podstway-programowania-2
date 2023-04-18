#include "mmenu.h"
#include "dbhandle.h"
#include <assert.h>
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

//#define _NDEBUG

#define MENUMARK (" * ")

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

int computeMenuWidth(const char *const title, const char *const choices[],
                     const int optionsCount) {
  assert(choices != NULL);
  assert(choices[0] != NULL);
  // assume title is longest (don't count mark)
  long unsigned longestChoiceLength = strlen(title) - strlen(MENUMARK);
  for (int i = 0; i < optionsCount; ++i) {
    const int temp = strlen(choices[i]);
    if (temp > longestChoiceLength)
      longestChoiceLength = temp;
  }

  // +2 for boarders, +2 because menu leaves 2 empty columns
  int windowCols = strlen(MENUMARK) + longestChoiceLength + 2 + 2;
  // make title centered
  if ((windowCols ^ strlen(title) & 1))
    ++windowCols;

  return windowCols;
}

void printMenuInWindow(WINDOW *window, const char *const title) {
  box(window, 0, 0);

  mvwprintw(window, 1, (getmaxx(window) - (strlen(title))) / 2, "%s", title);
  mvwaddch(window, 2, 0, ACS_LTEE);
  mvwhline(window, 2, 1, ACS_HLINE, getmaxx(window) - 2);
  mvwaddch(window, 2, getmaxx(window) - 1, ACS_RTEE);
}

void mainMenuSelection(void) {
  const char *const title = "Main menu";
  const char *const choices[] = {"Cars", "Clients", "Rentals", "Exit"};
  const int choicesCount = sizeof(choices) / sizeof(choices[0]);
  void (*menuFun[])(void) = {carsMenu, clientsMenu, rentalsMenu, NULL};

  // Instantiate items for menu
  ITEM **mainMenuItems = calloc(choicesCount + 1, sizeof(ITEM *));
  for (int i = 0; i < choicesCount; ++i) {
    mainMenuItems[i] = new_item(choices[i], choices[i]);
    set_item_userptr(mainMenuItems[i], menuFun[i]);
  }
  mainMenuItems[choicesCount] = NULL;

  const int windowCols = computeMenuWidth(title, choices, choicesCount);
  // boarders(3) + title(1) + choices count
  const int windowRows = choicesCount + 4;

  WINDOW *mainMenuWindow =
      newwin(windowRows, windowCols, (LINES - windowRows) / 2,
             (COLS - windowCols) / 2);
  keypad(mainMenuWindow, TRUE);
  PANEL *panel = new_panel(mainMenuWindow);

  MENU *mainMenu = new_menu(mainMenuItems);
  set_menu_win(mainMenu, mainMenuWindow);
  // -4 for boarders and title, start leave 3 lines for
  // boarders and title , and leave left boarder alone.
  set_menu_sub(mainMenu,
               derwin(mainMenuWindow, choicesCount, windowCols - 4, 3, 1));
  set_menu_mark(mainMenu, MENUMARK);
  set_menu_items(mainMenu, mainMenuItems);
  menu_opts_off(mainMenu, O_SHOWDESC);

  printMenuInWindow(mainMenuWindow, title);

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
#ifndef _NDEBUG
      // printing choices on stdscr for testing.
      move(LINES - 2, 0);
      clrtoeol();
      printw("MAIN MENU SELECTED: %s", name);
      getch();
#endif
      // EXIT has null pointer, break the switch and loop
      if (item_userptr(curitem) == NULL) {
        doExit = TRUE;
        break;
      }
      hide_panel(panel);
      ((void (*)(void))(item_userptr(curitem)))();
      show_panel(panel);
    }
  }

  unpost_menu(mainMenu);
  del_panel(panel);
  delwin(menu_sub(mainMenu));
  delwin(menu_win(mainMenu));
  free_menu(mainMenu);
  for (int i = 0; i < choicesCount; ++i)
    free_item(mainMenuItems[i]);
  free(mainMenuItems);
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
