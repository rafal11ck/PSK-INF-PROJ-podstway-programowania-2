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

void menuInvoke(const char *const title, const char *const choices[],
                const int choicesCount, void (*menuFun[])(void));

int getLongestStringLength(const char *const stringArr[],
                           const int stringsCount);

void formInvoke(FORM *form, const char *const formFieldNames[],
                const char *const title);

FORM *formInit(const int fieldCount);

void formFree(FORM *form);

#endif // MENUUTIL_H
