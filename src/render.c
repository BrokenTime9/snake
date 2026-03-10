#include "../include/game.h"
#include <ncurses.h>

void gameRender(Game *g, WINDOW *win) {
  short x = g->width;
  short y = g->height;

  char grass[] = {'.', ',', '\'', '`'};

  for (short i = 0; i < y; i++) {
    for (short j = 0; j < x; j++) {

      // snake vision

      if (INDEX(g, j, i).tileType == SPACE) {

        mvwaddch(win, i, j, ' ' | COLOR_PAIR(1));
      }

      if (INDEX(g, j, i).tileType == FRUIT) {
        mvwaddch(win, i, j, ' ' | COLOR_PAIR(2));
      }

      if (INDEX(g, j, i).tileType == SNAKE) {
        mvwaddch(win, i, j, ' ' | COLOR_PAIR(3));
      }

      if (INDEX(g, j, i).tileType == HEAD) {
        mvwaddch(win, i, j, ' ' | COLOR_PAIR(4));
      }

      if (INDEX(g, j, i).tileType == WALL) {
        mvwaddch(win, i, j, '.' | COLOR_PAIR(5));
      }
      if (INDEX(g, j, i).tileType == VISION) {
        mvwaddch(win, i, j, '-' | COLOR_PAIR(6));
      }
    }

    // change to loop
    mvwprintw(win, y + 1, 2, "Head X: %d  ", g->snake[0].headPos.tilex);
    mvwprintw(win, y + 2, 2, "Head Y: %d  ", g->snake[0].headPos.tiley);
    mvwprintw(win, y + 1, 15, "Length : %d  ", g->snake[0].length);
    if (g->snake[0].length > 1) {
      // mvwprintw(win, y + 1, 15, "Body X : %d  ", g->snake.body[1].tilex);
      // mvwprintw(win, y + 2, 15, "Body Y : %d  ", g->snake.body[1].tiley);
    }
    // mvwprintw(win, y + 2, 15, "Fruits : %d  ", fEats);
  }

  box(win, 0, 0);
  wrefresh(win);
  return;
}
