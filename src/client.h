#ifndef CLIENT_H_
#define CLIENT_H_

/**
 * @file
 * @brief Interface for Client structure.
 *
 * @todo Document propose of this file.
 **/

/**
 *@brief Indicates that Client::m_ID is not valid.
 **/
#define INVALIDCLIENTID -1

/**
 * @brief Structure holding client single data
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

struct Client *clientNew();
void clientFree(struct Client *client);

#endif // CLIENT_H_
