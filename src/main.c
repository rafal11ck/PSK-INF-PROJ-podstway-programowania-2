#include "dbhandle.h"
#include "mmenu/mmenu.h"

/**
 *@file
 *@brief Main file.
 * */
/**
 * @brief main.
 * @return 0;
 * */
int main() {
  if (!dbHandleOpenDB())
    mainMenu();
}
