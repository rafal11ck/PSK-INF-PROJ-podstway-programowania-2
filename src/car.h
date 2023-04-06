
/**
 * @brief Structure contaiong information about single car
 */
struct Car {
  //! registraction number.
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
