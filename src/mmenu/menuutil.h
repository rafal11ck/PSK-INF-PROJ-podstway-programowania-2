#ifndef MENUUTIL_H
#define MENUUTIL_H

/**
 *@file
 *@brief Header file for menu operations
 **/

#include <form.h>

/**
 *@brief amount of columns of the form field.
 **/
#define FORMFIELDLENGTH 40

void invokeMenu(const char *const title, const char *const choices[],
                const int choicesCount, void (*menuFun[])(void));

int getLongestStringLength(const char *const stringArr[],
                           const int stringsCount);

void invokeForm(FORM *form, const char *const title,
                const char *const formFieldNames[]);

#endif // MENUUTIL_H
