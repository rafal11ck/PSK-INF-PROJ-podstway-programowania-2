#ifndef MENUUTIL_H
#define MENUUTIL_H

/**
 *@file
 *
 **/

void invokeMenu(const char *const title, const char *const choices[],
                const int choicesCount, void (*menuFun[])(void));

#endif // MENUUTIL_H
