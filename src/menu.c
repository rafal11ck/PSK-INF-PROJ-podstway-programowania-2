#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void mainMenu();
void carsMenu();
void clientsMenu();
void rentalsMenu();

int main() {
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  mainMenu();
  endwin();
  return 0;
}

void mainMenu() {
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
  int wybor = 0;
  char *opcje[] = {"listRents", "addRent", "returnRent", "returnToMainMenu"};
  int liczba_opcji = sizeof(opcje) / sizeof(opcje[0]);
  int i;

  do {
    clear();
    refresh();
    printw("Rents Menu:\n");

    // Wyswietl opcje menu
    for (i = 0; i < liczba_opcji; i++) {
      if (i == wybor) {
        printw(" -> %s\n", opcje[i]);
      } else {
        printw("    %s\n", opcje[i]);
      }
    }

    // Odczytaj nacisniety klawisz
    int klawisz = getch();

    if (klawisz == 224) {
      klawisz = getch();
      if (klawisz == 72 && wybor > 0) { // Strzalka w gore
        wybor--;
      } else if (klawisz == 80 && wybor < liczba_opcji - 1) { // Strzalka w dol
        wybor++;
      }
    } else if (klawisz == 13) { // ENTER
      switch (wybor) {
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
        break;
      }

      // Zakoncz menu, jesli uzytkownik wybral opcje "returnToMainMenu"
      if (wybor == liczba_opcji - 1) {
        break;
      }

      // Poczekaj na nacisniecie dowolnego klawisza, aby wrocic do menu
      printw("Naciœnij dowolny klawisz, aby kontynuowaæ...");
      getch();
    }
  } while (1);
}
