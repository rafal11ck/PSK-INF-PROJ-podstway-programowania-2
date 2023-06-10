#include "dbhandle.h"
#include "clientsmenu.h"
#include "carsmenu.h"
#include "list.h"
#include <assert.h>
#include <car.h>
#include <client.h>
#include <curses.h>
#include <menuutil.h>
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
                                   "	yOfProd	INTEGER,"
                                   "	color	TEXT,"
                                   "	mileage	INTEGER,"
                                   "   PRIMARY KEY(ID AUTOINCREMENT));"
                                   "CREATE TABLE IF NOT EXISTS 'clients' ("
                                   "   'ID'	INTEGER NOT NULL UNIQUE,"
                                   "   'cardID'	INTEGER,"
                                   "   'name'	TEXT,"
                                   "   'surname'	TEXT,"
                                   "   'adress'   TEXT,"
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
 *@brief Ensure that database exists.
 *@return False if ok. True if failed.
 *Ensures that database exists and has required tables.
 **/
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
  sqlite3_close(DB);
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
          "VALUES (%d, '%s', '%s', '%s', %d);",
          client->m_cardID, client->m_name, client->m_surname, client->m_adress,
          client->m_phoneNum);
  return true;
}

/**
 * @brief Given car, get string containing SQL query that would add that car to DB.
 *
 * @param out Where to save the query.
 * @param car Car based on which the statement is generated.
 * @return true if the statement is created successfully, false otherwise.
 */
static bool dbHandleGetCarInsertQuery(char **out, const struct Car *car) {
  assert(*out);
  if (!carIsComplete(car)) {
    return false;
  }

  sprintf(*out,
          "INSERT INTO cars (regNum, brand, model, yOfProd, color, mileage) "
          "VALUES ('%s', '%s', '%s', %d, '%s', %ld);",
          car->m_regNum, car->m_brand, car->m_model, car->m_yOfProd, car->m_color, car->m_mileage);
  return true;
}

/**
 *@brief Insert client into db.
 *@param client Client to insert into db.
 *@return
 *- true if added client sucessfuly.
 *- false if failed.
 **/
bool dbHandleClientInsert(const struct Client *client) {
  sqlite3_open(DBFILENAME, &DB); // open
  char *err = NULL;
  char *query = calloc(500, sizeof(char));
  bool status = true;
  if (dbHandleGetClientInsertQuery(&query, client)) {
    int rc = sqlite3_exec(DB, query, NULL, NULL, &err);
    if (rc != SQLITE_OK) {
      const char *msg[] = {err, NULL};
      menuUtilMessagebox("dbHandleClientInsert failed", msg);
      sqlite3_free(err);
    }
  }
  free(query);
  sqlite3_close(DB);
  return true;
}

/**
 * @brief Insert a car into the database.
 *
 * @param car Car to insert into the database.
 * @return true if the car is added successfully, false if it fails.
 */
bool dbHandleCarInsert(const struct Car *car) {
  sqlite3_open(DBFILENAME, &DB);
  char *err = NULL;
  char *query = calloc(500, sizeof(char));
  bool status = true;
  if (dbHandleGetCarInsertQuery(&query, car)) {
    int rc = sqlite3_exec(DB, query, NULL, NULL, &err);
    if (rc != SQLITE_OK) {
      const char *msg[] = {err, NULL};
      menuUtilMessagebox("dbHandleCarInsert failed", msg);
      sqlite3_free(err);
    }
  }
  free(query);
  sqlite3_close(DB);
  return true;
}

/**
 *@brief Given query and callback function create List based on the query.
 *@param out Where result List is stored.
 *@param callback Function insering ListNode into List based on data returned
 *from sqlite. First parameter is pointer to List.
 *@param query Query to run in order to recive data.
 *@return
 *- true if sucess.
 *- false otherwise.
 **/
bool dbHandleGetResultAsList(struct List **out,
                             int (*callback)(void *list, int argc, char **argv,
                                             char **colNames),
                             const char *query) {
  if (SQLITE_OK != sqlite3_open(DBFILENAME, &DB)) {
    abort();
  }
  char *err = NULL;
  bool status = true;
  // if passed list is null ptr acllocate it.
  if (*out == NULL)
    *out = listCreateList();
  int rc = sqlite3_exec(DB, query, callback, *out, &err);
  if (rc != SQLITE_OK) {
    const char *msg[] = {err, NULL};
    menuUtilMessagebox("dbHandleGetResultAsList", msg);
    sqlite3_free(err);
    status = 0;
  }
  sqlite3_close(DB);
  return status;
}

/**
 *@brief Remove clinet from database.
 *@param id Id of clinet that will be removed
 *@return -- true if success -- false otherwise.
 *
 */
bool dbHandlClientRemove(int id) {
  sqlite3_open(DBFILENAME, &DB); // open
  char *err = NULL;
  char *query = calloc(500, sizeof(char));
  //! i left here
  sprintf(query, "DELETE FROM clients WHERE clients.ID=%d", id);

  bool status = true;
  int rc = sqlite3_exec(DB, query, NULL, NULL, &err);
  if (rc != SQLITE_OK) {
    const char *msg[] = {err, NULL};
    menuUtilMessagebox("dbHandleRemoveClient", msg);
    sqlite3_free(err);
    status = false;
  }
  // free(query); // ??
  sqlite3_close(DB);
  return status;
}

/**
 * @brief Remove a car from the database.
 *
 * @param id ID of the car to be removed.
 * @return true if successful, false otherwise.
 */
bool dbHandleCarRemove(int id) {
  sqlite3_open(DBFILENAME, &DB);
  char *err = NULL;
  char *query = calloc(500, sizeof(char));
  sprintf(query, "DELETE FROM cars WHERE cars.ID=%d", id);

  bool status = true;
  int rc = sqlite3_exec(DB, query, NULL, NULL, &err);
  if (rc != SQLITE_OK) {
    const char *msg[] = {err, NULL};
    menuUtilMessagebox("dbHandleCarRemove", msg);
    sqlite3_free(err);
    status = false;
  }
  sqlite3_close(DB);
  return status;
}

/**
 *@brief Update Client in database.
 *@param toEdit Client to update.
 *@return Whather succeded
 *- true -- sucess
 *- false -- failed
 **/
bool dbHandleClientUpdate(struct Client *toEdit) {
  char *err = NULL;
  char *query = calloc(700, sizeof(char));
  sprintf(query,
          "UPDATE clients SET cardID = '%d', name = '%s', surname "
          "='%s', adress='%s', phoneNumber='%d' WHERE ID = %d;",
          toEdit->m_cardID, toEdit->m_name, toEdit->m_surname, toEdit->m_adress,
          toEdit->m_phoneNum, toEdit->m_ID);
  bool status = true;
  sqlite3_open(DBFILENAME, &DB); // open
  int rc = sqlite3_exec(DB, query, NULL, NULL, &err);
  if (rc != SQLITE_OK) {
    const char *msg[] = {err, NULL};
    assert(err);
    menuUtilMessagebox("dbHandleClientUpdate ERROR", msg);
    sqlite3_free(err);
    status = false;
  }
  // free(query); // ??
  sqlite3_close(DB);
  return status;
}

/**
 * @brief Update a car in the database.
 *
 * @param toEdit Car to update.
 * @return true if successful, false otherwise.
 */
bool dbHandleCarUpdate(struct Car *toEdit) {
  char *err = NULL;
  char *query = calloc(700, sizeof(char));
  sprintf(query,
          "UPDATE cars SET regNum = '%s', brand = '%s', model = '%s', "
          "yOfProd = %d, color = '%s', mileage = %ld WHERE ID = %d;",
          toEdit->m_regNum, toEdit->m_brand, toEdit->m_model, toEdit->m_yOfProd,
          toEdit->m_color, toEdit->m_mileage, toEdit->m_ID);
  bool status = true;
  sqlite3_open(DBFILENAME, &DB);
  int rc = sqlite3_exec(DB, query, NULL, NULL, &err);
  if (rc != SQLITE_OK) {
    const char *msg[] = {err, NULL};
    assert(err);
    menuUtilMessagebox("dbHandleCarUpdate ERROR", msg);
    sqlite3_free(err);
    status = false;
  }
  sqlite3_close(DB);
  return status;
}
