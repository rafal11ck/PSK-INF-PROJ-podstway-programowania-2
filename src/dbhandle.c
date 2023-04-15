#include "dbhandle.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

/**
 *@breif Database path.
 * */
const char *DBFILENAME = "database.db";

/**
 *@brief SQL query ensuring tables needed for program to run exist.
 **/
char *ENUSREDBTABLESQUERY = "CREATE TABLE IF NOT EXISTS cars("
                            "	ID	INTEGER NOT NULL,"
                            "	regNum	TEXT,"
                            "	brand	TEXT,"
                            "	model	TEXT,"
                            "	yearOfProduction	TEXT,"
                            "	color	TEXT,"
                            "	mileage	INTEGER,"
                            "   PRIMARY KEY(ID AUTOINCREMENT));"
                            "   CREATE TABLE IF NOT EXISTS 'clients' ("
                            "   'ID'	INTEGER NOT NULL UNIQUE,"
                            "   'cardID'	INTEGER,"
                            "   'name'	TEXT,"
                            "   'surname'	TEXT,"
                            "   'phoneNumber'	INTEGER,"
                            "   PRIMARY KEY('ID' AUTOINCREMENT)"
                            "   );"
                            "   CREATE TABLE IF NOT EXISTS 'rentals' ("
                            "   'ID'	INTEGER NOT NULL,"
                            "   'clientID'	TEXT NOT NULL,"
                            "   'carID'	TEXT NOT NULL,"
                            "   'since'	TEXT,"
                            "   'until'	TEXT,"
                            "   PRIMARY KEY('ID' AUTOINCREMENT)"
                            "   );";

/**
 *@brief Database connection;
 * */
sqlite3 *DB;

/**
 * @brief Compiled statement for sqlite.
 * */
sqlite3 *STMT;

bool dbHandleOpenDB() {
  sqlite3_open(DBFILENAME, &DB); // oen database
  char *err;
  int rc = sqlite3_exec(DB, ENUSREDBTABLESQUERY, NULL, NULL, &err);
  // if failed return true
  if (rc != SQLITE_OK) {
    fprintf(stderr, "%s\n", err);
    return true;
  }
  return false;
}
