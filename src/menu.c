#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

void mainMenu();
void carsMenu();
void clientsMenu();
void rentalsMenu();

int main() {
   mainMenu();
   return 0;
}

void mainMenu() {
   int wybor = 0;
   char *opcje[] = {"Cars", "Clients", "Rentals", "Exit"};
   int liczba_opcji = sizeof(opcje)/sizeof(opcje[0]);
   int i;

   do {
      system("cls");
      printf("Main Menu:\n");

      // Wyœwietl opcje menu
      for(i = 0; i < liczba_opcji; i++) {
         if(i == wybor) {
            printf(" -> %s\n", opcje[i]);
         }
         else {
            printf("    %s\n", opcje[i]);
         }
      }

      // Odczytaj naciœniêty klawisz
      int klawisz = getch();

      if(klawisz == 224) {
         klawisz = getch();
         if(klawisz == 72 && wybor > 0) { // Strza³ka w górê
            wybor--;
         }
         else if(klawisz == 80 && wybor < liczba_opcji - 1) { // Strza³ka w dó³
            wybor++;
         }
      }
      else if(klawisz == 13) { // ENTER
         switch(wybor) {
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
               printf("Do widzenia!\n");
               break;
         }

         // Zakoñcz program, jeœli u¿ytkownik wybra³ opcjê "Wyjœcie"
         if(wybor == liczba_opcji - 1) {
            break;
         }

         // Poczekaj na naciœniêcie dowolnego klawisza, aby wróciæ do menu
         printf("Naciœnij dowolny klawisz, aby kontynuowaæ...");
         getch();
      }
   } while(1);
}

void carsMenu() {
       int wybor = 0;
   char *opcje[] = {"listCars", "searchCars", "addCars", "removeCars", "editCars", "returnToMainMenu"};
   int liczba_opcji = sizeof(opcje)/sizeof(opcje[0]);
   int i;

   do {
      system("cls");
      printf("Cars Menu:\n");

      // Wyœwietl opcje menu
      for(i = 0; i < liczba_opcji; i++) {
         if(i == wybor) {
            printf(" -> %s\n", opcje[i]);
         }
         else {
            printf("    %s\n", opcje[i]);
         }
      }

      // Odczytaj naciœniêty klawisz
      int klawisz = getch();

      if(klawisz == 224) {
         klawisz = getch();
         if(klawisz == 72 && wybor > 0) { // Strza³ka w górê
            wybor--;
         }
         else if(klawisz == 80 && wybor < liczba_opcji - 1) { // Strza³ka w dó³
            wybor++;
         }
      }
      else if(klawisz == 13) { // ENTER
         switch(wybor) {
            case 0:
               printf("Wybrales opcje: listCars\n");
               break;
            case 1:
               printf("Wybrales opcje: searchCars\n");
               break;
            case 2:
               printf("Wybrales opcje: addCars\n");
               break;
            case 3:
               printf("Wybrales opcje: removeCars\n");
               break;
            case 4:
               printf("Wybrales opcje: editCars\n");
               break;
            case 5:
               break;
         }

         // Zakoñcz program, jeœli u¿ytkownik wybra³ opcjê "Wyjœcie"
         if(wybor == liczba_opcji - 1) {
            break;
         }

         // Poczekaj na naciœniêcie dowolnego klawisza, aby wróciæ do menu
         printf("Naciœnij dowolny klawisz, aby kontynuowaæ...");
         getch();
      }
   } while(1);
}

void clientsMenu() {
       int wybor = 0;
   char *opcje[] = {"listClients", "addClients", "removeClients", "editClients", "returnToMainMenu"};
   int liczba_opcji = sizeof(opcje)/sizeof(opcje[0]);
   int i;

   do {
      system("cls");
      printf("Clients Menu:\n");

      // Wyœwietl opcje menu
      for(i = 0; i < liczba_opcji; i++) {
         if(i == wybor) {
            printf(" -> %s\n", opcje[i]);
         }
         else {
            printf("    %s\n", opcje[i]);
         }
      }

      // Odczytaj naciœniêty klawisz
      int klawisz = getch();

      if(klawisz == 224) {
         klawisz = getch();
         if(klawisz == 72 && wybor > 0) { // Strza³ka w górê
            wybor--;
         }
         else if(klawisz == 80 && wybor < liczba_opcji - 1) { // Strza³ka w dó³
            wybor++;
         }
      }
      else if(klawisz == 13) { // ENTER
         switch(wybor) {
            case 0:
               printf("Wybrales opcje: listClients\n");
               break;
            case 1:
               printf("Wybrales opcje: addClients\n");
               break;
            case 2:
               printf("Wybrales opcje: removeClients\n");
               break;
            case 3:
               printf("Wybrales opcje: editClients\n");
               break;
            case 4:
               break;
         }

         // Zakoñcz program, jeœli u¿ytkownik wybra³ opcjê "Wyjœcie"
         if(wybor == liczba_opcji - 1) {
            break;
         }

         // Poczekaj na naciœniêcie dowolnego klawisza, aby wróciæ do menu
         printf("Naciœnij dowolny klawisz, aby kontynuowaæ...");
         getch();
      }
   } while(1);
}

void rentalsMenu() {
       int wybor = 0;
   char *opcje[] = {"listRents", "addRent", "returnRent", "returnToMainMenu"};
   int liczba_opcji = sizeof(opcje)/sizeof(opcje[0]);
   int i;

   do {
      system("cls");
      printf("Rents Menu:\n");

      // Wyœwietl opcje menu
      for(i = 0; i < liczba_opcji; i++) {
         if(i == wybor) {
            printf(" -> %s\n", opcje[i]);
         }
         else {
            printf("    %s\n", opcje[i]);
         }
      }

      // Odczytaj naciœniêty klawisz
      int klawisz = getch();

      if(klawisz == 224) {
         klawisz = getch();
         if(klawisz == 72 && wybor > 0) { // Strza³ka w górê
            wybor--;
         }
         else if(klawisz == 80 && wybor < liczba_opcji - 1) { // Strza³ka w dó³
            wybor++;
         }
      }
      else if(klawisz == 13) { // ENTER
         switch(wybor) {
            case 0:
               printf("Wybrales opcje: listRents\n");
               break;
            case 1:
               printf("Wybrales opcje: addRent\n");
               break;
            case 2:
               printf("Wybrales opcje: returnRent\n");
               break;
            case 3:
               break;
         }

         // Zakoñcz program, jeœli u¿ytkownik wybra³ opcjê "Wyjœcie"
         if(wybor == liczba_opcji - 1) {
            break;
         }

         // Poczekaj na naciœniêcie dowolnego klawisza, aby wróciæ do menu
         printf("Naciœnij dowolny klawisz, aby kontynuowaæ...");
         getch();
      }
   } while(1);
}
