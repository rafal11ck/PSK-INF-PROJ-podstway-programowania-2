#include "client.h"

/**
 * @brief Contains infomation about rental.
 */
struct Rental {
  //! \link Client::m_cardId Card id of client \endlink who rents car.
  int m_clientCardID;
  //! \link Car::m_regNum Car registraction number \endlink of rented car.
  char *m_carReg;
  //! Rental start date.
  char *m_since;
  //! Rental end date.
  char *m_untill;
};
