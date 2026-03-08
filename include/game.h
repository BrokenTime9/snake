#ifndef GAME_H
#define GAME_H
#include "enum.h"
#include "ncurses.h"

#define INDEX(g, x, y) g->tiles[(y) * g->width + (x)]

typedef struct {
  short tilex; // for x cord of tile -- cant be unsigned as cords go both ways
  short tiley; // for y cord of tile
} Cords;

typedef struct {
  char snakeName;

  Cords headPos; // head cord
  Cords *body;   // some Cords for body
  int length;

  Cords *vision; // some Cords from head that indicate how far the snake can see
  char visionL;
  Direction direction; // direction of movement

  unsigned char health; // health of sanke
} Snake;

typedef struct {
  Cords pos;
} Fruit;

typedef struct {
  Cords tileno; // tiles of playground

  Tile tileType; // for the snake to check whats the tile for possible movement

} Playground;

typedef struct {
  Playground *tiles;

  Snake *snake;
  Fruit fruit;

  char snakeCnt;
  char snakeCntCap;
  char snakeCap;

  short width;
  short height;

} Game;

void cursesInit();
void gameInit(Game *g, short x, short y);
void snakeInit(Game *g);
void fruitInit(Game *g);
void gameRender(Game *g, WINDOW *win);

void gameUpdate(Game *g, WINDOW *win);

#endif
