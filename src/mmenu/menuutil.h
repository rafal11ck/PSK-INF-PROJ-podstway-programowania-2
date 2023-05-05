#ifndef MENUUTIL_H
#define MENUUTIL_H

/**
 *@file
 *@brief Header file for menu operations
 **/

#include "list.h"
#include <form.h>

/**
 *@brief How many columns should forms have.
 *
 *Also affects ListViwer.
 **/
#define FORMFIELDLENGTH 40

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

void listViewInvoke(void **out,
                    void (*extractData)(void **out,
                                        const struct ListNode *const data),
                    struct List *(*listFuns[])(),
                    const char *const columnNames[], const int colCount,
                    char *(*getItemString)(void *),
                    void (*dealloactor)(void *));

#endif // MENUUTIL_H
