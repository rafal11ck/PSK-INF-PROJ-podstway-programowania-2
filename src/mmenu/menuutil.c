#include "menuutil.h"
#include "../list.h" //! @todo remove .., LSP complaining.
#include <assert.h>
#include <eti.h>
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

//! @todo Display IMPORTANT it has to be list as menu items with usr pointers to
//! nodes in the list, use pads for scrolling horizontally.

/**
 *@brief String to indicate current selected choice in menus.
 **/
#define MENUMARK (" * ")

/**
 *@brief How many columns should form field have.
 **/
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
  do {
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
      attron(COLOR_PAIR(1));
      printw("SELECTED: %s", name);
      attroff(COLOR_PAIR(1));
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
  } while (!doExit);
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
 *@brief Manages input in the form
 *@param form form that input will go into .
 *@todo Resign form form, change return type.
 **/
static void formHandleIteraction(FORM *form) {
  bool doExit = false;
  int tmp; // < C98 moment

  do {
    update_panels();
    doupdate();
    int input = getch();
    switch (input) {
    case 10:
      tmp = form_driver(form, REQ_DOWN_FIELD);
#ifndef NDEBUG
      attron(COLOR_PAIR(1));
      mvprintw(LINES - 4, 0, "form_driver status code = %d", tmp);
      attroff(COLOR_PAIR(1));
#endif
      if (tmp == E_INVALID_FIELD) {
        break;
        //! @todo Display information about invalid data in current field.
      }
      doExit = true;
      form_driver(form, REQ_END_LINE);
      break;
    case KEY_DOWN:
      form_driver(form, REQ_DOWN_FIELD);
      form_driver(form, REQ_END_LINE);
      break;
    case KEY_UP:
      form_driver(form, REQ_UP_FIELD);
      form_driver(form, REQ_END_LINE);
      break;
    case KEY_LEFT:
      form_driver(form, REQ_PREV_CHAR);
      break;
    case KEY_RIGHT:
      form_driver(form, REQ_NEXT_CHAR);
      break;
    // Delete the char before cursor
    case KEY_BACKSPACE:
    case 127:
      form_driver(form, REQ_DEL_PREV);
      break;
    // Delete the char under the cursor
    case KEY_DC:
      form_driver(form, REQ_DEL_CHAR);
      break;
    default:
      form_driver(form, input);
      break;
    }
  } while (!doExit);
}

/**
 *@brief Put form on screen in nice looking form.
 *@param form Form that will be put on scree.
 *@param formFieldNames array of field names.
 *@param title Title of window(form)
 *
 **/
void formInvoke(FORM *form, const char *const formFieldNames[],
                const char *const title) {
  assert(form);
  assert(title);
  assert(formFieldNames);
  // form sub window rows
  int subRows;
  // form sub window cols
  int subCols;
  scale_form(form, &subRows, &subCols);

  const int titleLenght = strlen(title);
  // How many columns are needed for field names.
  const int fieldNamesColsNeeded =
      getLongestStringLength(formFieldNames, field_count(form));
  // +4 for boarders
  const int formWinCols =
      max(titleLenght, fieldNamesColsNeeded + FORMFIELDLENGTH) + 4;
  // Rows will be rows needed for fields + 3 for boarders + 1 row for title
  const int formWinRows = subRows + 3 + 1;

  WINDOW *formWin = newwin(formWinRows, formWinCols, (LINES - formWinRows) / 2,
                           (COLS - formWinCols) / 2);
  keypad(formWin, true);
  set_form_win(form, formWin);
  // start at 3 (2 boarders + title row)
  // +1 space for nice looking
  set_form_sub(form, derwin(form_win(form), subRows, subCols, 3,
                            fieldNamesColsNeeded + 1));

  PANEL *panel = new_panel(formWin);

  printWindowBoarders(form_win(form), title);
  for (int i = 0; i < field_count(form); ++i) {
    mvwprintw(form_win(form), 3 + i * 2, 1, "%s", formFieldNames[i]);
  }
  post_form(form);

  formHandleIteraction(form);

  del_panel(panel);
  delwin(form_sub(form));
  delwin(form_win(form));
  unpost_form(form);
#ifndef NDEBUG
  // print content of form on screen.
  attron(COLOR_PAIR(1));
  mvprintw(1, 1, "%s", title);
  for (int i = 0; i < field_count(form); ++i) {
    mvprintw(2 + i, 1, "%s (changed: %s): %s", formFieldNames[i],
             field_status(form_fields(form)[i]) ? "yes" : "no",
             field_buffer(form_fields(form)[i], 0));
  }
  attroff(COLOR_PAIR(1));
#endif
}

/**
 *@brief Initialize FORM.
 *@param fieldCount How many fields will be in the field.
 *@return initialized FORM pointer.
 **/
FORM *formInit(const int fieldCount) {
  // allocate
  FIELD **field = calloc(sizeof(FIELD *), fieldCount + 1);
  field[fieldCount] = NULL;
  for (int i = 0; i < fieldCount; ++i) {
    field[i] = new_field(1, FORMFIELDLENGTH, 2 * i, 1, 0, 0);
    assert(field[i]);
    set_field_back(field[i], A_UNDERLINE);
    field_opts_off(field[i], O_AUTOSKIP);
  }
  FORM *form = new_form(field);

  return form;
}

/**
 *@brief Frees memory used for form, and it's fields.
 *@param form Form to be free.
 **/
void formFree(FORM *form) {
  unpost_form(form);
  FIELD **fields = form_fields(form);
  const int fieldCount = field_count(form);
  free_form(form);
  for (int i = 0; i < fieldCount; ++i) {
    free_field(fields[i]);
  }
  free(fields);
}

/**
 *@brief Allocates and fills MENU, based on List content.
 *@param list List based on which MENU will be created.
 *@param getItem Creates ITEM based on ListNode::m_data(it's passed as
 *praemeter).
 */
static MENU *listViewMakeMenu(struct List *list, ITEM *(getItem)(void *)) {
  // Allocate memory for MENU choices.
  ITEM **menuItems = calloc(listSize(list), sizeof(ITEM *));
  {
    // Fill ITEMs with data from list.
    int i = 0;
    for (struct ListNode *it = listGetFront(list); it != NULL;
         it = it->m_next, ++i) {
      //! @warning Does not use weaper around ListNode to get
      //! ListNode::m_data that is passsed to getItem function as parameter.
      menuItems[i] = getItem(it->m_data);
    }
    // After loop i is one after last element where our sentinel should be.
    menuItems[i] = NULL;
  }
  //! @todo implement
  MENU *menu = NULL;
  return menu;
}

/**
 *@brief List Viewer for lists.
 *@param out Where result will be saved.
 *@param extractData Function taking two parameters first is pointer to the
 *memory where result will be saved (out parameter will be passed
 *internally), second is Listnode, from witch data will be extracted.
 *@param listFuns array of functions that return sorted list.
 *@param columnNames array of column names strings.
 *@param colCount How many columns are there.
 *@param getItem Creates ITEM based on ListNode::m_data(it's passed as
 *praemeter).
 *@todo implement.
 */
void listViewInvoke(void **out,
                    void (*extractData)(void **out,
                                        const struct ListNode *const data),
                    struct List *(*listFuns[])(void),
                    const char *const columnNames[], const int colCount,
                    ITEM *(*getItem)(void *)) {
  assert(out && "No result destnation given.");
  assert(extractData && "Can't extract data.");
  assert(listFuns && "No list functions passed");
  assert(getItem && "Can't create list without that function.");

  // Load List
  struct List *list = listFuns[0]();

  // 2. display list

  // 3. given chosen menu item call extractData on it.
  // 4. free memory.
  // 5. return wanted data.
}
