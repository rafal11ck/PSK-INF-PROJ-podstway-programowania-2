#ifndef CLIENT_H_
#define CLIENT_H_

#define INVALIDCLIENTID -1

/**
 * @brief Structure hodling client single data
 */
struct Client {
  //! %Client ID.
  int m_ID;
  //! %Client's card ID.
  int m_cardID;
  //! String holding first name.
  char *m_name;
  //! String holding second name.
  char *m_surname;
  //! Address.
  char *m_adress;
  //! Phone number.
  int m_phoneNum;
};

#endif // CLIENT_H_
