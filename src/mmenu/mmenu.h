#ifndef MMENU_H
#define MMENU_H
#include <ncurses.h>

/**
 *@file
 *@brief For invoking menu.
 **/

/**
 * @brief Invokes menu.
 * */
void mainMenu();

/**
 *@brief Print boarders of window
 *@param window WINDOW pointer
 *@param title Char pointer to title of menu
 *
 *Prints box around window, at top puts title that is also boxed, and rest
 *of space is left unchanged.
 **/
void printWindowBoarders(WINDOW *window, const char *const title);

#endif // MMENU_H
