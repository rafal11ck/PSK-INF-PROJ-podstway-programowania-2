#ifndef DBHANDLE_H
#define DBHANDLE_H
#include <stdbool.h>

/**
 *@brief Opens connection with database.
 *@return False if ok. True if failed.
 *Ensures that database exists and has required tables.
 **/
bool dbHandleOpenDB();

#endif // DBHANDLE_H
