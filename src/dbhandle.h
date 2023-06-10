#ifndef DBHANDLE_H
#define DBHANDLE_H
#include "car.h"
#include "client.h"
#include <stdbool.h>
/**
 *@file
 *@brief Database operations interface
 **/

/**
 *@brief Opens connection with database.
 *@return False if ok. True if failed.
 *Ensures that database exists and has required tables.
 **/
bool dbHandleOpenDB();

static bool dbHandleGetClientInsertQuery(char **out, const struct Client *client);
static bool dbHandleGetCarInsertQuery(char **out, const struct Car *car);

bool dbHandleClientInsert(const struct Client *client);
bool dbHandleCarInsert(const struct Car *car);

bool dbHandleGetResultAsList(struct List **out,
                             int (*callback)(void *list, int argc, char **argv,
                                             char **colNames),
                             const char *query);

bool dbHandlClientRemove(int id);
bool dbHandleCarRemove(int id);

bool dbHandleClientUpdate(struct Client *toEdit);
bool dbHandleCarUpdate(struct Car *toEdit);

#endif // DBHANDLE_H
