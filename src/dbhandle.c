#include "dbhandle.h"
#include <assert.h>
#include <client.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *@file
 *@brief Database handling
 **/

/**
 *@brief Database path.
 * */
static const char *DBFILENAME = "database.db";

/**
 *@brief SQL query ensuring tables needed for program to run exist.
 **/
static char *ENUSREDBTABLESQUERY = "CREATE TABLE IF NOT EXISTS cars("
                                   "	ID	INTEGER NOT NULL,"
                                   "	regNum	TEXT,"
                                   "	brand	TEXT,"
                                   "	model	TEXT,"
                                   "	yearOfProduction	TEXT,"
                                   "	color	TEXT,"
                                   "	mileage	INTEGER,"
                                   "   PRIMARY KEY(ID AUTOINCREMENT));"
                                   "CREATE TABLE IF NOT EXISTS 'clients' ("
                                   "   'ID'	INTEGER NOT NULL UNIQUE,"
                                   "   'cardID'	INTEGER,"
                                   "   'name'	TEXT,"
                                   "   'surname'	TEXT,"
                                   "   'adress'   TEXT"
                                   "   'phoneNumber'	INTEGER,"
                                   "   PRIMARY KEY('ID' AUTOINCREMENT)"
                                   "   );"
                                   "CREATE TABLE IF NOT EXISTS 'rentals' ("
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
static sqlite3 *DB;

/**
 * @brief Compiled statement for sqlite.
 * */
static sqlite3 *STMT;

/**
 *@brief Opens connection to database
 */
bool dbHandleOpenDB() {
  sqlite3_open(DBFILENAME, &DB); // open database
  char *err;
  int rc = sqlite3_exec(DB, ENUSREDBTABLESQUERY, NULL, NULL, &err);
  // if failed return true
  if (rc != SQLITE_OK) {
    fprintf(stderr, "%s\n", err);
    sqlite3_free(err);
    return true;
  }
  return false;
}

/**
 *@brief Given client, get string containing SQL query that would add that
 *client to DB.
 *@param out Where to save query.
 *@warning This function does not allocate space for out.
 *@param client Client based on which statemnet is generated.
 *@return
 *- true if created statement succesfully
 *- false otherwise.
 **/
static bool dbHandleGetClientInsertQuery(char **out,
                                         const struct Client *client) {
  assert(*out);
  if (!clientIsComplete(client)) {
    return false;
  }

  sprintf(*out,
          "INSERT INTO clients (cardID, name, surname, adress, phoneNumber) "
          "VALUES %d, '%s', '%s', '%s', %d);",
          client->m_cardID, client->m_name, client->m_surname, client->m_adress,
          client->m_phoneNum);
  return true;
}

bool ddbHandleClientInsert(const struct Client *client) {
  // sqlite3_open(DBFILENAME, &DB); // open
  char *err;
  char *query = calloc(500, sizeof(char));
  if (dbHandleGetClientInsertQuery(&query, client)) {
    int rc = sqlite3_exec(DB, query, NULL, NULL, &err);
    if (rc != SQLITE_OK) {
      fprintf(stderr, "%s", err);
      sqlite3_free(err);
    }
  }
  free(query);
  return true;
}
