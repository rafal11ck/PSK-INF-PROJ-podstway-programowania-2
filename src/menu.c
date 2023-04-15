#include "menu.h"
#include "dbhandle.h"
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 *@file
 *@brief Menu implementation.
 **/

/**
 * @brief Handles main menu.
 */
void mainMenuSelection();

/**
 * @brief Handles cars menu.
 * */
void carsMenu();

/**
 * @brief Handles clients menu.
 * */
void clientsMenu();

/**
 * @brief Handles rentals menu.
 * */
void rentalsMenu();

void mainMenu() {
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

void mainMenuSelection() {
  char *opcje[] = {"Cars", "Clients", "Rentals", "Exit"};
  const int liczbaOpcji = sizeof(opcje) / sizeof(opcje[0]);

  int currentChoice = 0;
  int choice;
  while (true) {
    clear();
    mvprintw(0, 0, "Main Menu:\n");

    for (int i = 0; i < liczbaOpcji; i++) {
      // print arrow for current option
      if (i == currentChoice)
        printw(" -> ");
      else
        printw("    ");
      printw("%s\n", opcje[i]);
    }

    refresh();
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
        carsMenu();
        break;
      case 1:
        clientsMenu();
        break;
      case 2:
        rentalsMenu();
        break;
      case 3:
        return;
      }
    }
  }
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
