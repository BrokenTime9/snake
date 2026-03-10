#include "../include/game.h"

static const Cords direction[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

void tileUpdate(Game *g, Tile type, Cords old, Cords new, char mode) {
  short x = g->width;

  int tempOldInd = old.tiley * x + old.tilex;
  int tempNewInd = new.tiley * x + new.tilex;

  if (mode == 2) {
    if (type != SPACE) {
      g->tiles[tempOldInd].tileType = SPACE;
    }

    g->tiles[tempNewInd].tileType = type;
  }

  if (mode == 1) {

    g->tiles[tempNewInd].tileType = type;
  }

  /*
  if (t == SPACE) {
    game.tiles[tempNewInd].tileType = SPACE;
  }

  if (t == SNAKE) {
    game.tiles[tempOldInd].tileType = SPACE;
    game.tiles[tempNewInd].tileType = SNAKE;
  }

  if (t == FRUIT) {
    game.tiles[tempOldInd].tileType = SPACE;
    game.tiles[tempNewInd].tileType = FRUIT;
  }
  */
  return;
};

void snakeMoveUpdate(Game *g, Cords *old, Snake *s) {
  short x = g->width;
  short y = g->height;

  short oldx = old->tilex;
  short oldy = old->tiley;

  short newx = oldx + direction[s->direction].tilex;
  short newy = oldy + direction[s->direction].tiley;

  tileUpdate(g, HEAD, *old, (Cords){newx, newy}, 2);

  s->headPos.tilex = newx;
  s->headPos.tiley = newy;

  oldx = s->body[0].tilex;
  oldy = s->body[0].tiley;

  s->body[0].tilex = newx;
  s->body[0].tiley = newy;

  for (char i = 1; i < s->length; i++) {

    short tempx = s->body[i].tilex;
    short tempy = s->body[i].tiley;
    tileUpdate(g, SNAKE, (Cords){tempx, tempy}, (Cords){oldx, oldy}, 2);

    s->body[i].tilex = oldx;
    s->body[i].tiley = oldy;

    oldx = tempx;
    oldy = tempy;
  }

  return;
}

void gameUpdate(Game *g, WINDOW *win) {
  short x = g->width;
  short y = g->height;

  Cords fruit = g->fruit.pos;

  char i;
  for (i = 0; i < g->snakeCnt; i++) {
    Snake *s = &g->snake[i];
    snakeUpdate(g, s);
  }

  gameRender(g, win);
  return;
}
