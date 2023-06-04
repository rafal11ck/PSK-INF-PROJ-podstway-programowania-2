#ifndef CARSMENU_H
#define CARSMENU_H

/**
 *@file
 *@brief Cars menu interface
 *
 **/

#include "car.h"

void carsMenu(void);

char *carGetListViewString(const struct Car *car);
#endif // CARSMENU_H
