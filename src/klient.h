#ifndef KLIENT_H_
#define KLIENT_H_

/**
 * @brief Structure hodling client data
 * */
struct Client {
  int m_id;        //! Unique id
  char *m_name;    //! String holding first name
  char *m_surname; //! String holding second name
};

#endif // KLIENT_H_
