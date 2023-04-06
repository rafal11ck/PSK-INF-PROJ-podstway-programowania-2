#ifndef KLIENT_H_
#define KLIENT_H_

/**
 * @brief Structure hodling client data
 * */
struct Client {
  //! Unique card ID
  int m_cardId;
  //! String holding first name
  char *m_name;
  //! String holding second name
  char *m_surname;
  //! Adress
  char *m_adress;
  //! phone number
  int m_phone;
};

#endif // KLIENT_H_
