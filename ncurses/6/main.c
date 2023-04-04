#include <ncurses.h>

int main() {
  initscr();
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  printw("A big string some thing don't care");
  mvchgat(0, 0, -1, A_BLINK, 1, NULL);
  refresh();
  getch();
  endwin();
  return 0;
}
