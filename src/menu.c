#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

void mainMenu();
void carsMenu();
void clientsMenu();
void rentalsMenu();

int main() {
  initscr();            // Inicjalizacja ekranu ncurses
  cbreak();             // Wylaczenie buforowania linii
  noecho();             // Wylaczenie wyswietlania wczytywanych znakow
  keypad(stdscr, TRUE); // Wlaczenie obslugi klawiatury

  mainMenu();

  endwin(); // Zakonczenie trybu ncurses
  return 0;
}

void mainMenu() {
  int wybor = 0;
  char *opcje[] = {"Cars", "Clients", "Rentals", "Exit"};
  int liczba_opcji = sizeof(opcje) / sizeof(opcje[0]);
  int i;

  do {
    clear(); // Wyczyszczenie ekranu

    // Wyswietl opcje menu
    printw("Main Menu:\n");
    for (i = 0; i < liczba_opcji; i++) {
      if (i == wybor) {
        printw(" -> ");
      } else {
        printw("    ");
      }
      printw("%s\n", opcje[i]);
    }

    // Odczytaj nacisniety klawisz
    int klawisz = getch();

    if (klawisz == KEY_UP && wybor > 0) {
      wybor--; // Strzalka w gore
    } else if (klawisz == KEY_DOWN && wybor < liczba_opcji - 1) {
      wybor++;                  // Strzalka w dol
    } else if (klawisz == 10) { // ENTER
      switch (wybor) {
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
        printw("Do widzenia!\n");
        break;
      }

      // Zakoncz program, jesli uzytkownik wybral opcje "Wyjscie"
      if (wybor == liczba_opcji - 1) {
        break;
      }

      // Poczekaj na nacisniecie dowolnego klawisza, aby wrocic do menu
      printw("Nacisnij dowolny klawisz, aby kontynuowac...");
      getch();
    }
  } while (1);
}

void carsMenu() {
  int wybor = 0;
  char *opcje[] = {"listCars",   "searchCars", "addCars",
                   "removeCars", "editCars",   "returnToMainMenu"};
  int liczba_opcji = sizeof(opcje) / sizeof(opcje[0]);
  int i;

  do {
    clear(); // Wyczyszczenie ekranu

    // Wyswietl opcje menu
    printw("Cars Menu:\n");
    for (i = 0; i < liczba_opcji; i++) {
      if (i == wybor) {
        printw(" -> ");
      } else {
        printw("    ");
      }
      printw("%s\n", opcje[i]);
    }

    // Odczytaj nacisniety klawisz
    int klawisz = getch();

    if (klawisz == KEY_UP && wybor > 0) {
      wybor--; // Strzalka w gore
    } else if (klawisz == KEY_DOWN && wybor < liczba_opcji - 1) {
      wybor++;                  // Strzalka w dol
    } else if (klawisz == 10) { // ENTER
      switch (wybor) {
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
        break;
      }

      // Zakoncz program, jesli uzytkownik wybral opcje "Wyjscie"
      if (wybor == liczba_opcji - 1) {
        break;
      }

      // Poczekaj na nacisniecie dowolnego klawisza, aby wrocic do menu
      printw("Nacisnij dowolny klawisz, aby kontynuowac...");
      getch();
    }
  } while (1);
}

void clientsMenu() {
  int wybor = 0;
  char *opcje[] = {"listClients", "addClients", "removeClients", "editClients",
                   "returnToMainMenu"};
  int liczba_opcji = sizeof(opcje) / sizeof(opcje[0]);
  int i;

  do {
    clear(); // Wyczyszczenie ekranu

    // Wyswietl opcje menu
    printw("Clients Menu:\n");
    for (i = 0; i < liczba_opcji; i++) {
      if (i == wybor) {
        printw(" -> ");
      } else {
        printw("    ");
      }
      printw("%s\n", opcje[i]);
    }

    // Odczytaj nacisniety klawisz
    int klawisz = getch();

    if (klawisz == KEY_UP && wybor > 0) {
      wybor--; // Strzalka w gore
    } else if (klawisz == KEY_DOWN && wybor < liczba_opcji - 1) {
      wybor++;                  // Strzalka w dol
    } else if (klawisz == 10) { // ENTER
      switch (wybor) {
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
        break;
      }

      // Zakoncz program, jesli uzytkownik wybral opcje "Wyjscie"
      if (wybor == liczba_opcji - 1) {
        break;
      }

      // Poczekaj na nacisniecie dowolnego klawisza, aby wrocic do menu
      printw("Nacisnij dowolny klawisz, aby kontynuowac...");
      getch();
    }
  } while (1);
}

void rentalsMenu() {
  int wybor = 0;
  char *opcje[] = {"listRents", "addRent", "returnRent", "returnToMainMenu"};
  int liczba_opcji = sizeof(opcje) / sizeof(opcje[0]);
  int i;

  do {
    clear(); // Wyczyszczenie ekranu

    // Wyswietl opcje menu
    printw("Rentals Menu:\n");
    for (i = 0; i < liczba_opcji; i++) {
      if (i == wybor) {
        printw(" -> ");
      } else {
        printw("    ");
      }
      printw("%s\n", opcje[i]);
    }

    // Odczytaj nacisniety klawisz
    int klawisz = getch();

    if (klawisz == KEY_UP && wybor > 0) {
      wybor--; // Strzalka w gore
    } else if (klawisz == KEY_DOWN && wybor < liczba_opcji - 1) {
      wybor++;                  // Strzalka w dol
    } else if (klawisz == 10) { // ENTER
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

      // Zakoncz program, jesli uzytkownik wybral opcje "Wyjscie"
      if (wybor == liczba_opcji - 1) {
        break;
      }

      // Poczekaj na nacisniecie dowolnego klawisza, aby wrocic do menu
      printw("Nacisnij dowolny klawisz, aby kontynuowac...");
      getch();
    }
  } while (1);
}
