#ifndef CLEINTSMENU_H
#define CLEINTSMENU_H

/**
 *@file
 *@brief Clients menu interface.
 **/

#include "client.h"

void clientsMenu(void);

char *clientGetListViewString(struct Client *client);
#endif // CLEINTSMENU_H
