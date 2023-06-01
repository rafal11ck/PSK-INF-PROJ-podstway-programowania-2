#include "car.h"
#include "dbhandle.h"
#include "list.h"
#include "menuutil.h"
#include <assert.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *@file
 *@brief Implements function to operate on client.
 **/

/**
 * @brief Allocates and returns car containing nothing.
 * @return Car object.
 *
 * C style strings are NULL pointers.
 * Car::m_ID is INVALIDCARID.
 * */
struct Car *carNew() {
  struct Car *result = calloc(sizeof(struct Car), 1);
  result->m_ID = INVALIDCARID;
  result->m_yOfProd = INVALIDCARYOFPROD;
  result->m_mileage = INVALIDCARMILEAGE;
  return result;
}

/**
 * @brief Deallocates car.
 * @param car Car to be freed.
 */
void carFree(struct Car *car) {
  free(car->m_regNum);
  free(car->m_brand);
  free(car->m_model);
  free(car->m_color);
  free(car);
}

/**
 * @brief Checks if every field in Car is set.
 * @param car Car to check.
 * @return
 * - True if Car is complete.
 * - False otherwise.
 */
bool carIsComplete(const struct Car *car) {
  bool result = true;
  if (!car || car->m_regNum == NULL || car->m_brand == NULL ||
      car->m_model == NULL ||car->m_yOfProd == INVALIDCARYOFPROD ||
      car->m_color == NULL || car->m_mileage == INVALIDCARMILEAGE)
    result = false;
  return result;
}

/**
 * @brief CallBack function for dbHandleGetResultAsList.
 * Transforms row into Car.
 * @param list List to insert data into.
 * @param argc How many columns are there.
 * @param argv Values in columns.
 * @param colNames names of columns.
 * @return 0.
 */
static int carGetListQueryCallback(struct List *list, int argc,
                                   const char **argv,
                                   const char **const colNames) {
  assert(list && argv && argc && colNames);
  struct Car *car = carNew();
  for (int i = 0; i < argc; ++i) {
    const char *colName = colNames[i];
    const char *val = argv[i];
    if (!strcmp(colName, "ID")) {
      car->m_ID = atoi(val);
    } else if (!strcmp(colName, "regNum")) {
      car->m_regNum = calloc(FORMFIELDLENGTH + 1, sizeof(char));
      strcpy(car->m_regNum, val);
    } else if (!strcmp(colName, "brand")) {
      car->m_brand = calloc(FORMFIELDLENGTH + 1, sizeof(char));
      strcpy(car->m_brand, val);
    } else if (!strcmp(colName, "model")) {
      car->m_model = calloc(FORMFIELDLENGTH + 1, sizeof(char));
      strcpy(car->m_model, val);
    } else if (!strcmp(colName, "yOfProd")) {
      car->m_yOfProd = atoi(val);
    } else if (!strcmp(colName, "color")) {
      car->m_color = calloc(FORMFIELDLENGTH + 1, sizeof(char));
      strcpy(car->m_color, val);
    } else if (!strcmp(colName, "mileage")) {
      car->m_mileage = atol(val);
    } else {
      fprintf(stderr, "Car to structure fail. FAILED on %s", colName);
      abort();
    }
  }
  listPushBack(list, car);
  return 0;
}

/**
 * @brief Generates SQL query.
 * @param sType CarSort based on which to sort.
 * @param desc Whether sorting should be descending.
 * - false -- ascending
 * - true -- descending
 * @return query.
 */
char *carGetQueryOfSort(int sType, bool desc) {
  char *query = calloc(1000, sizeof(char));
  strcpy(query,
         "SELECT ID, regNum, brand, model, yOfProd, color, mileage FROM cars "
         "ORDER BY ");
  assert(sType >= 0 && sType < carSort_MAX);
  char *orderStr = NULL;
  switch (sType) {
    case carSort_regNum:
      orderStr = "regNum";
      break;
    case carSort_brand:
      orderStr = "brand";
      break;
    case carSort_model:
      orderStr = "model";
      break;
    case carSort_yOfProd:
      orderStr = "yOfProd";
      break;
    case carSort_color:
      orderStr = "color";
      break;
    case carSort_mileage:
      orderStr = "mileage";
      break;
  }
  strcat(query, orderStr);
  if (desc)
    strcat(query, " DESC");
  strcat(query, ";");
  return query;
}

/**
 * @brief Get list of cars.
 * @param sType Sort type corresponding to CarSort.
 * @param desc Whether sorting should be descending.
 * - false -- ascending
 * - true -- descending
 * @return List of cars.
 */
struct List *carGetList(int sType, bool desc) {
  struct List *res = NULL;
  char *q = carGetQueryOfSort(sType, desc);
  dbHandleGetResultAsList(
      &res, (int (*)(void *, int, char **, char **))carGetListQueryCallback,
      q);
  return res;
}

/**
 * @brief Make a clone of Car. It allocates memory internally.
 * @param dest Car structure where data will be cloned into.
 * @param src Car to create a clone of.
 */
void carClone(struct Car **dest, const struct Car *src) {
  struct Car *res = NULL;

  res = carNew();
  res->m_ID = src->m_ID;

  res->m_yOfProd = src->m_yOfProd;

  res->m_mileage = src->m_mileage;

  res->m_regNum = calloc(FORMFIELDLENGTH + 1, sizeof(char));
  strcpy(res->m_regNum, src->m_regNum);

  res->m_brand = calloc(FORMFIELDLENGTH + 1, sizeof(char));
  strcpy(res->m_brand, src->m_brand);

  res->m_model = calloc(FORMFIELDLENGTH + 1, sizeof(char));
  strcpy(res->m_model, src->m_model);

  res->m_color = calloc(FORMFIELDLENGTH + 1, sizeof(char));
  strcpy(res->m_color, src->m_color);

  *dest = res;
}

