#ifndef MENUUTIL_H
#define MENUUTIL_H

/**
 *@file
 *@brief Header file for menu operations
 **/

void invokeMenu(const char *const title, const char *const choices[],
                const int choicesCount, void (*menuFun[])(void));

#endif // MENUUTIL_H
