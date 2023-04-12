#include "client.h"

/**
 * @brief Contains infomation about rental.
 */
struct Rental {
  //! Rental Id.
  long long m_rentalID;
  //! \link Client::m_ID Card id of client \endlink who rents car.
  long long m_clientID;
  //! \link Car::m_regNum Car registraction number \endlink of rented car.
  char *m_carRegNum;
  //! Rental start date.
  char *m_since;
  //! Rental end date.
  char *m_untill;
};
