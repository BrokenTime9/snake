#include "../include/game.h"
#include <curses.h>
#include <stdlib.h>
#include <time.h>

void gameFree(Game *g) {
  free(g->tiles);

  for (char i = 0; i < g->snakeCnt; i++) {
    free(g->snake[i].body);
    free(g->snake[i].vision);
  }
  free(g->snake);
}

int main() {
  Game game;

  srand(time(NULL));

  cursesInit();

  WINDOW *win = newwin(LINES, COLS, 0, 0);

  gameInit(&game, COLS, LINES - 10);
  snakeInit(&game);
  fruitInit(&game);
  gameRender(&game, win);
  wtimeout(win, 100);

  while (1) {
    int ch = wgetch(win);
    // custom move <-> ai move - comment out to  manual
    // customMoveUpdate(&game, ch);

    gameUpdate(&game, win);

    if (game.snake->headPos.tilex == 1 || game.snake->headPos.tiley == 1) {
      break;
    }
    if (game.snake->headPos.tilex == game.width - 2 ||
        game.snake->headPos.tiley == game.height - 1) {
      break;
    }
    if (ch == 'q')
      break;
    if (ch == 'r') {
      fruitInit(&game);
      gameRender(&game, win);
    }
  }

  gameFree(&game);
  endwin();
  return 0;
}
