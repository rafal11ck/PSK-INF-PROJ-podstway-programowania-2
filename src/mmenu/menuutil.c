#include "menuutil.h"
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
 *@brief Menu displaying utilities.
 *
 *Wraps ncurses liblary.
 **/

/**
 *@brief String to indicate current selected choice in menus.
 **/
#define MENUMARK (" * ")

static int computeMenuWidth(const char *const title,
                            const char *const choices[],
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

static void printMenuInWindow(WINDOW *window, const char *const title) {
  box(window, 0, 0);

  mvwprintw(window, 1, (getmaxx(window) - (strlen(title))) / 2, "%s", title);
  mvwaddch(window, 2, 0, ACS_LTEE);
  mvwhline(window, 2, 1, ACS_HLINE, getmaxx(window) - 2);
  mvwaddch(window, 2, getmaxx(window) - 1, ACS_RTEE);
}

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
}

void invokeMenu(const char *const title, const char *const choices[],
                const int choicesCount, void (*menuFun[])(void)) {
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
