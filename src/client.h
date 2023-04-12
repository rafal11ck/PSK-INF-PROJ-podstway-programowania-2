#ifndef CLIENT_H_
#define CLIENT_H_

/**
 * @brief Structure hodling client single data
 */
struct Client {
  //! %Client ID.
  int m_clientId;
  //! %Client's card ID.
  int m_cardId;
  //! String holding first name.
  char *m_name;
  //! String holding second name.
  char *m_surname;
  //! Adress.
  char *m_adress;
  //! Phone number.
  int m_phoneNum;
};

#endif // CLIENT_H_
