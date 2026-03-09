#include "../include/game.h"
#include "ncurses.h"
#include "stdlib.h"
#include <locale.h>

void cursesInit() {
  // initial ncurses stuff
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  // colors
  start_color();

  /*
  init_color(COLOR_RED, 145, 600, 400);
  init_color(COLOR_GREEN, 790, 50, 64);
  init_color(COLOR_BLUE, 214, 725, 600);
*/
  // 1 space 2 fruit 3 snake 4 head 5 wall

  init_pair(3, COLOR_BLACK, COLOR_GREEN);
  init_pair(2, COLOR_GREEN, COLOR_RED);
  init_pair(1, COLOR_GREEN, COLOR_WHITE);
  init_pair(4, COLOR_GREEN, COLOR_BLACK);
  init_pair(5, COLOR_GREEN, COLOR_BLUE);

  return;
}

void gameInit(Game *g, short x, short y) {

  g->tiles = malloc(sizeof(Playground) * x * y);
  g->snake = malloc(sizeof(Snake) * 10);

  y = y - (y * 0.01); // lower window

  g->width = x;
  g->height = y;

  g->snakeCap = 20;
  g->snakeCnt = 0;
  g->snakeCntCap = 10;

  for (short i = 0; i < y; i++) {
    for (short j = 0; j < x; j++) {
      INDEX(g, j, i).tileno.tilex = j;
      INDEX(g, j, i).tileno.tiley = i;

      if (i == 1) {
        INDEX(g, j, i).tileType = WALL;
      } else if (i == y - 1) {
        INDEX(g, j, i).tileType = WALL;
      } else if (j == 1) {
        INDEX(g, j, i).tileType = WALL;
      } else if (j == x - 2) {
        INDEX(g, j, i).tileType = WALL;
      } else {
        INDEX(g, j, i).tileType = SPACE;
      }
    }
  }

  return;
}

void snakeInit(Game *g) {
  short x = g->width;
  short y = g->height;

  // snake init
  short sx = (rand() % (x - 2)) + 1;
  short sy = (rand() % (y - 2)) + 1;

  short tempCnt = g->snakeCnt;
  g->snakeCnt++;

  INDEX(g, sx, sy).tileno.tiley = sy;
  INDEX(g, sx, sy).tileno.tilex = sx;
  INDEX(g, sx, sy).tileType = HEAD;

  g->snake[tempCnt].snakeName = 1;
  g->snake[tempCnt].headPos.tilex = sx;
  g->snake[tempCnt].headPos.tiley = sy;
  g->snake[tempCnt].health = 20;
  g->snake[tempCnt].length = 1;
  g->snake[tempCnt].visionL = 4;

  g->snake[tempCnt].body = malloc(sizeof(Cords) * g->snakeCap);
  g->snake[tempCnt].body[0].tilex = sx;
  g->snake[tempCnt].body[0].tiley = sy;

  return;
};

void fruitInit(Game *g) {
  short x = g->width;
  short y = g->height;

  // fruit init
  if (g->fruit.pos.tilex) {
    short tempx, tempy, tempInd;
    tempx = g->fruit.pos.tilex;
    tempy = g->fruit.pos.tiley;
    tempInd = tempy * x + tempx;

    INDEX(g, tempx, tempy).tileType = SPACE;
  }

  short rx, ry, fruitpos;
  // keep looping so fruit does not erase snake
  rx = (rand() % (x - 2)) + 1;
  ry = (rand() % (y - 2)) + 1;

  if (INDEX(g, rx, ry).tileType == SNAKE) {
    rx = rx + 1;
    ry = ry + 1;
  };

  fruitpos = ry * x + rx;

  g->fruit.pos.tilex = rx;
  g->fruit.pos.tiley = ry;
  g->tiles[fruitpos].tileType = FRUIT;

  return;
}
