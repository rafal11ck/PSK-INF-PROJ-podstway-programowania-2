#include "clientsmenu.h"
#include "menuutil.h"
#include "mmenu.h"
#include <form.h>
#include <ncurses.h>
#include <panel.h>
#include <stdlib.h>

/**
 *@file
 *@brief Clients menu implementation.
 */

/**
 *\todo Implement
 *
 *@brief FORM for editing client;
 **/
void addClient() {
  const char *const title = "Add client";
  const char *const fieldnames[] = {"cos1", "cos2", "fdgjdf"};
  const int fieldCount = sizeof(fieldnames) / sizeof(*fieldnames);
  FIELD *fields[fieldCount + 1]; // 1+ for NULL delimiter.
  for (int i = 0; i < fieldCount; ++i) {
    // i*2 so there is empty line
    fields[i] = new_field(1, 10, 6 + i * 2, 1, 0, 0);
    set_field_back(fields[i], A_UNDERLINE);
  }
  fields[fieldCount - 1] = NULL; // set delimiter
  int minRows = 0;
  int minCols = 0;

  FORM *form = new_form(fields);
  scale_form(form, &minRows, &minCols);
  WINDOW *formWin = newwin(minRows + 4, minCols + 4, (LINES - minRows) / 2,
                           (COLS - minCols) / 2);
  keypad(formWin, TRUE);
  set_form_win(form, formWin);
  set_form_sub(form, derwin(formWin, minRows, minCols, 3, 1));

  PANEL *formPanel = new_panel(formWin);
  printWindowBoarders(form_win(form), title);
  post_form(form);

  bool doExit = FALSE;
  while (!doExit) {
    update_panels();
    doupdate();
    int input = getch();
    switch (input) {
    case KEY_UP:
      form_driver(form, REQ_UP_FIELD);
      break;
    case KEY_DOWN:
      form_driver(form, REQ_DOWN_FIELD);
      break;
    default:
      form_driver(form, input);
      break;
    }
  }
  unpost_form(form);
  delwin(form_sub(form));
  delwin(form_win(form));
  free_form(form);
  for (int i = 0; i < fieldCount; ++i) {
    free_field(fields[i]);
  }
}

/**
 *@brief Handles displaying of clients menu.
 */
void clientsMenu(void) {

  const char *const title = "Clients";
  const char *const choices[] = {"listClients", "Add client", "removeClients",
                                 "editClients", "Return to main menu"};
  const int choicesCount = sizeof(choices) / sizeof(choices[0]);
  //! @todo impelemnt submenus
  void (*menuFun[])(void) = {NULL, addClient, NULL, NULL, NULL};
  invokeMenu(title, choices, choicesCount, menuFun);
}
