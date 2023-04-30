#ifndef CLEINTSMENU_H
#define CLEINTSMENU_H

/**
 *@file
 *@brief Clients menu interface.
 **/

void clientsMenu(void);
struct Client *clientNew();
void clientFree(struct Client *client);
#endif // CLEINTSMENU_H
