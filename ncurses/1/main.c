#include <ncurses.h>
// Podczas linkowania trzeba dać "-lncurses" (gcc)

int main() {
  initscr();
  printw("Hello World !!!");
  refresh();
  getch();
  endwin();
  return 0;
}
