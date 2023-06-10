#ifndef CAR_H_
#define CAR_H_

/**
 * @file
 * @brief Interface for Car structure.
 *
 **/

#include "list.h"
#include <stdbool.h>

/**
 *@brief Indicates that Car::m_ID is not valid.
 **/
#define INVALIDCARID -1
/**
 *@brief Indicates that Car::m_yOfProd is not valid.
 **/
#define INVALIDCARYOFPROD -1
/**
 *@brief Indicates that Car::m_mileage is not valid.
 **/
#define INVALIDCARMILEAGE -1

/**
 * @brief Structure containing information about car.
 */
struct Car {
  //! %Car ID.
  int m_ID;
  //! registration number.
  char *m_regNum;
  //! Make/brand of the car.
  char *m_brand;
  //! model.
  char *m_model;
  //! Year of production.
  short m_yOfProd;
  //! color.
  char *m_color;
  //! car mileage in KM.
  long m_mileage;
};

/**
 *@brief Car sort types.
 **/
enum CarSort {
  //! registration number
  carSort_regNum,
  //! Make/brand of the car
  carSort_brand,
  //! model
  carSort_model,
  //! Year of production
  carSort_yOfProd,
  //! color
  carSort_color,
  //! car mileage in KM.
  carSort_mileage,
  //! how many of CarSort types exist
  carSort_MAX
};

struct Car *carNew();
void carFree(struct Car *car);

bool carIsComplete(const struct Car *car);

char *carGetQueryOfSort(int sType, bool desc);
struct List *carGetList(int sType, bool desc);
void carClone(struct Car **dest, const struct Car *src);
#endif // CAR_H_
