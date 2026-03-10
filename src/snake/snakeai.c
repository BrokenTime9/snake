#include "../../include/game.h"
#include <ncurses.h>
#include <stdlib.h>

static const Cords direction[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

void snakeRandMove(Game *g, Snake *s) {
  Cords snake = s->headPos;

  short m = rand() % 4;

  switch (m) {
  case 0:
    s->direction = TOP;
    break;
  case 1:
    s->direction = LEFT;
    break;
  case 2:
    s->direction = RIGHT;
    break;
  case 3:
    s->direction = BOTTOM;
    break;
  }

  snakeMoveUpdate(g, &snake, s);
};

void snakeVision(Game *g, Snake *s) {

  Direction dir = s->direction;

  short heady = s->headPos.tiley + direction[dir].tiley;
  short headx = s->headPos.tilex + direction[dir].tilex;

  short visionR = 0;

  for (short i = 0; i < s->visionA; i++) {
    if (s->vision[i].tiley > 1 && s->vision[i].tiley < g->height - 1 &&
        s->vision[i].tilex > 1 && s->vision[i].tilex < g->width - 2) {
      if (INDEX(g, s->vision[i].tilex, s->vision[i].tiley).tileType != FRUIT) {

        tileUpdate(g, SPACE, s->vision[i], s->vision[i], 1);
      }
    }
  }

  for (short x = 0; x <= s->visionL; x++) {
    if (visionR >= s->visionA)
      break;
    Cords temp = add(headx, heady, direction[dir], x);

    s->vision[visionR] = temp;
    visionR++;

    if (dir == TOP || dir == BOTTOM) {
      for (int y = 1; y <= x; y++) {
        s->vision[visionR] = add(temp.tilex, temp.tiley, direction[LEFT], y);

        visionR++;
      }
      for (int y = 1; y <= x; y++) {

        s->vision[visionR] = add(temp.tilex, temp.tiley, direction[RIGHT], y);

        visionR++;
      }
    }

    if (dir == RIGHT || dir == LEFT) {
      for (int y = 1; y <= x; y++) {

        s->vision[visionR] = add(temp.tilex, temp.tiley, direction[TOP], y);

        visionR++;
      }
      for (int y = 1; y <= x; y++) {

        s->vision[visionR] =

            add(temp.tilex, temp.tiley, direction[BOTTOM], y);

        visionR++;
      }
    }
  }

  for (short i = 0; i < s->visionA; i++) {
    if (s->vision[i].tiley > 1 && s->vision[i].tiley < g->height - 1 &&
        s->vision[i].tilex > 1 && s->vision[i].tilex < g->width - 2) {

      if (INDEX(g, s->vision[i].tilex, s->vision[i].tiley).tileType != SNAKE &&
          INDEX(g, s->vision[i].tilex, s->vision[i].tiley).tileType != FRUIT) {
        tileUpdate(g, VISION, s->vision[i], s->vision[i], 1);
      }
    }
  }

  return;
}
void snakeUpdate(Game *g, Snake *s) {
  short x = g->width;
  short y = g->height;

  Cords fruit = g->fruit.pos;
  Cords snake = s->headPos;

  snakeVision(g, s);

  bool fruitVisible = isIn(fruit, s->vision, s->visionA);

  if (!fruitVisible) {
    snakeRandMove(g, s);
    snakeVision(g, s);
    return;
  }

  if (snake.tiley < fruit.tiley && fruitVisible) {
    s->direction = TOP;
  } else if (snake.tiley > fruit.tiley && fruitVisible) {
    s->direction = BOTTOM;
  } else if (snake.tilex < fruit.tilex && fruitVisible) {
    s->direction = LEFT;
  } else if (snake.tilex > fruit.tilex && fruitVisible) {
    s->direction = RIGHT;
  }

  snakeMoveUpdate(g, &snake, s);

  snake = s->headPos;
  if (fruit.tiley == snake.tiley && fruit.tilex == snake.tilex) {
    if (s->length < s->lengthCap) {
      s->length++;
    }

    fruitInit(g);
  }
  return;
}
