#include "dbhandle.h"
#include "mmenu/mmenu.h"

/**
 *@file
 *@brief Main file.
 * */
/**
 * @brief main.
 * */
int main() {
  if (!dbHandleOpenDB())
    mainMenu();
}
