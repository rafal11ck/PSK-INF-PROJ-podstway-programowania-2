#include "menuutil.h"
#include <assert.h>
#include <form.h>
#include <math.h>
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

#define FORMFIELDLENGTH 40

/**
 *@brief Add basic functionality to the shitty language which C is.
 *@param a First value to compare.
 *@param b Second value to compare.
 *@return Maximum of parameters passed.
 **/
int max(const int a, const int b) { return a > b ? a : b; }

/**
 *@brief Get length of longest string in array of strings.
 *@param stringArr Array of strings to look for longest string at.
 *@param stringsCount How many strings are in the array.
 *@return Length of longest string in array.
 **/
int getLongestStringLength(const char *const stringArr[],
                           const int stringsCount) {
  assert(stringArr);
  assert(stringsCount > 0);
  int result = strlen(stringArr[0]);
  for (int i = 0; i < stringsCount; ++i) {
    if (result < strlen(stringArr[i]))
      result = strlen(stringArr[i]);
  }
  return result;
}

/**
 *@brief Calculate minimum window width.
 *@param title Char pointer to title.
 *@param choices Char pointer to table of choices
 *@param optionsCount Number of elements in table of choices
 *@return Minimum number of columns needed
 **/
int computeWidth(const char *const title, const char *const choices[],
                 const int optionsCount) {
  assert(choices != NULL);
  assert(choices[0] != NULL);
  // assume title is longest (don't count mark)

  const int titleLength = strlen(title);
  const int choicesColsNeeded = getLongestStringLength(choices, optionsCount);

  const int colsNeeded =
      titleLength > choicesColsNeeded ? titleLength : choicesColsNeeded;

  // +2 for boarders, +2 because menu leaves 2 empty columns
  int windowCols = strlen(MENUMARK) + colsNeeded + 2 + 2;
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
static void menuHandleIteraction(MENU *menu, PANEL *panel) {
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
 *
 *@todo Split into functions, make allocation and dallcation seperate functions,
 *make it allocate on heap instead of stack.
 **/
void menuInvoke(const char *const title, const char *const choices[],
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

  menuHandleIteraction(mainMenu, panel);

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

/**
 *@brief Put form on screen in nice looking menu.
 *@param form Form that will be put on scree.
 *@param title Title of window(form)
 *
 **/
void formHandle(FORM *form, const char *const formFieldNames[],
                const char *const title) {
  assert(form);
  assert(title);
  assert(formFieldNames);
  // form subwindow rows
  int subRows;
  // form subwindow cols
  int subCols;
  scale_form(form, &subRows, &subCols);

  const int titleLenght = strlen(title);
  // How many columns are needed for field names.
  const int fieldNamesColsNeeded =
      getLongestStringLength(formFieldNames, field_count(form));
  const int formWinCols = max(titleLenght, fieldNamesColsNeeded);
  // Rows will be rows needed for fields + 3 for boarders + 1 row for title
  const int formWinRows = subRows + 3 + 1;

  getch();
  // WINDOW formWin = newwin();

  //! @todo implement
}

FORM *formInit(const int fieldCount) {
  // allocate
  FIELD **field = calloc(sizeof(FIELD *), fieldCount + 1);
  field[fieldCount - 1] = NULL;
  for (int i = 0; i < fieldCount - 1; ++i) {
    field[i] = new_field(1, FORMFIELDLENGTH, 2 * i, 1, 0, 0);
    assert(field[i]);
    set_field_back(field[i], A_UNDERLINE);
  }
  FORM *form = new_form(field);

  return form;
}

void formFree(FORM *form) {
  unpost_form(form);
  FIELD **fields = form_fields(form);
  const int fieldCount = field_count(form);
  free_form(form);
  free(fields);
  for (int i = 0; i < fieldCount; ++i) {
    free_field(fields[i]);
  }
}

/**
 *@brief Form with given fields and title.
 *@param formFieldsNames Field names of the form.
 *@param fieldCount How many fields will be in the form, size of formFieldNames
 *array.
 *@param title Title of the form.
 *
 *@todo form result parsing with function pointer as parameter.
 * */
void formInvoke(const char *const formFieldNames[], const int fieldCount,
                const char *const title) {
  assert(formFieldNames);
  assert(fieldCount > 0);
  assert(title);

  FORM *form = formInit(fieldCount);
  //! @todo make form go on screen
  formHandle(form, formFieldNames, title);
  //! @todo parse form

  // free memory
  formFree(form);
  return;
}
