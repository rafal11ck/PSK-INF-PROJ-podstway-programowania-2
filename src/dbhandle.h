#ifndef DBHANDLE_H
#define DBHANDLE_H
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

bool dbHandleClientInsert(const struct Client *client);

#endif // DBHANDLE_H
