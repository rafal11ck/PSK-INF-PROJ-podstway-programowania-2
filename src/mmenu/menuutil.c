#include "menuutil.h"
#include <assert.h>
#include <form.h>
#include <menu.h>
#include <ncurses.h>
#include <panel.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *@file
 *@brief Menu displaying utilities.
 *
 *Wraps ncurses liblary.
 **/

/**
 *@brief String to indicate current selected choice in menus.
 **/
#define MENUMARK (" * ")

/**
 *@brief Calculate minimum window width
 *@param title Char pointer to title of menu
 *@param choices Char pointer to table of choices
 *@param optionsCount Number of elements in table of choices
 *@return Minumum number of columns needed
 **/
int computeWidth(const char *const title, const char *const choices[],
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

void printWindowBoarders(WINDOW *window, const char *const title) {
  box(window, 0, 0);

  mvwprintw(window, 1, (getmaxx(window) - (strlen(title))) / 2, "%s", title);
  mvwaddch(window, 2, 0, ACS_LTEE);
  mvwhline(window, 2, 1, ACS_HLINE, getmaxx(window) - 2);
  mvwaddch(window, 2, getmaxx(window) - 1, ACS_RTEE);
}

/**
 *@brief Control menu navigation and invoke option that we chose
 *@param menu MENU pointer
 *@param panel PANEL pointer
 **/
static void handleMenuIteraction(MENU *menu, PANEL *panel) {
  int input;
  bool doExit = FALSE;
  while (!doExit) {
    update_panels();
    doupdate();
    input = getch();
    switch (input) {
    case KEY_UP:
      menu_driver(menu, REQ_UP_ITEM);
      break;
    case KEY_DOWN:
      menu_driver(menu, REQ_DOWN_ITEM);
      break;
    case 10:;
      ITEM *curitem = current_item(menu);
      const char *const name = item_name(curitem);
#ifndef NDEBUG
      // printing choices on stdscr for testing.
      move(LINES - 2, 0);
      clrtoeol();
      printw("SELECTED: %s", name);
      // getch();
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
}

/**
 *@brief Handle all operation and functions for menu
 *@param title Char pointer to title of menu
 *@param choices Char pointer to table of choices
 *@param choicesCount Number of elements in table of choices
 *@param menuFun Table of pointers on functions
 **/
void invokeMenu(const char *const title, const char *const choices[],
                const int choicesCount, void (*menuFun[])(void)) {
  // Instantiate items for menu
  ITEM **mainMenuItems = calloc(choicesCount + 1, sizeof(ITEM *));
  for (int i = 0; i < choicesCount; ++i) {
    mainMenuItems[i] = new_item(choices[i], choices[i]);
    set_item_userptr(mainMenuItems[i], menuFun[i]);
  }
  mainMenuItems[choicesCount] = NULL;

  const int windowCols = computeWidth(title, choices, choicesCount);
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

  printWindowBoarders(mainMenuWindow, title);

  post_menu(mainMenu);

  handleMenuIteraction(mainMenu, panel);

  // Deallocation
  unpost_menu(mainMenu);
  del_panel(panel);
  delwin(menu_sub(mainMenu));
  delwin(menu_win(mainMenu));
  free_menu(mainMenu);
  for (int i = 0; i < choicesCount; ++i)
    free_item(mainMenuItems[i]);
  free(mainMenuItems);
}

void invokeForm(const char *title, const char *const fieldNames[],
                const int fieldCount) {
  // Allocate Stuff
  FIELD *fields[fieldCount + 1]; // 1+ for NULL delimiter.
  for (int i = 0; i < fieldCount; ++i) {
    // i*2 so there is empty line
    fields[i] = new_field(1, 30, 1 + i * 2, 1, 0, 0);
    set_field_back(fields[i], A_UNDERLINE);
  }
  fields[fieldCount] = NULL; // set delimiter

  // Allocate Form
  FORM *form = new_form(fields);

  // Get minimum size needed for fields.
  int minRows = 0;
  int minCols = 0;
  scale_form(form, &minRows, &minCols);
  // Compute space needed for field names.
  const int colsNeeded = computeWidth(title, fieldNames, fieldCount);
  // winRows = minimum rows needed for fields + rows for boarders
  const int winRows = minRows + 4;
  // columns for fields + boarders and space + columns needed for field names.
  const int winCols = minCols + 4 + colsNeeded;

  WINDOW *formWin =
      newwin(winRows, winCols, (LINES - winRows) / 2, (COLS - winCols) / 2);
  keypad(formWin, TRUE);
  set_form_win(form, formWin);
  // 3 = boarder + title + boarder; 1 + colsNeeded is offest so that field
  // names and boarder fits.
  set_form_sub(form, derwin(formWin, minRows, minCols, 3, 1 + colsNeeded));

  // Drawing form
  PANEL *formPanel = new_panel(formWin);
  printWindowBoarders(form_win(form), title);
  for (int i = 0; i < fieldCount; ++i) {
    mvwprintw(form_win(form), 1 + 3 + i * 2, 2, "%s", fieldNames[i]);
  }
  post_form(form);
  // input handling in form
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
  // free memory
  unpost_form(form);
  delwin(form_sub(form));
  delwin(form_win(form));
  free_form(form);
  for (int i = 0; i < fieldCount; ++i) {
    free_field(fields[i]);
  }
}
