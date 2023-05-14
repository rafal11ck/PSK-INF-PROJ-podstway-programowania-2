#include "dbhandle.h"
#include "menuutil.h"
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
  const char *test[] = {"test", NULL};
  menuUtilMessagebox("TEST in main", test);
  if (!dbHandleOpenDB())
    mainMenu();
}
