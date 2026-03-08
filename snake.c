#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

#define INDEX(g, x, y) g->tiles[(y) * g->width + (x)]

typedef struct {
  short tilex; // for x cord of tile -- cant be unsigned as cords go both ways
  short tiley; // for y cord of tile
} Cords;

typedef enum {
  TOP,
  BOTTOM,
  LEFT,
  RIGHT,
} Direction;

typedef enum { HEAD, SNAKE, FRUIT, SPACE } Tile;

const Cords direction[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

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

char fEats = 0;

void gameInit(Game *g, short x, short y) {
  g->tiles = malloc(sizeof(Playground) * x * y);
  g->snake = malloc(sizeof(Snake) * 10);

  g->width = x;
  g->height = y;

  g->snakeCap = 20;
  g->snakeCnt = 0;
  g->snakeCntCap = 10;

  for (short i = 0; i < y; i++) {
    for (short j = 0; j < x; j++) {
      INDEX(g, j, i).tileno.tilex = j;
      INDEX(g, j, i).tileno.tiley = i;
      INDEX(g, j, i).tileType = SPACE;
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

void gameRender(Game *g, WINDOW *win) {
  short x = g->width;
  short y = g->height;

  for (short i = 1; i < y - 1; i++) {
    for (short j = 1; j < x - 1; j++) {

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
    }

    // change to loop
    mvwprintw(win, y + 1, 2, "Head X: %d  ", g->snake[0].headPos.tilex);
    mvwprintw(win, y + 2, 2, "Head Y: %d  ", g->snake[0].headPos.tiley);
    mvwprintw(win, y + 1, 15, "Length : %d  ", g->snake[0].length);
    if (g->snake[0].length > 1) {
      // mvwprintw(win, y + 1, 15, "Body X : %d  ", g->snake.body[1].tilex);
      // mvwprintw(win, y + 2, 15, "Body Y : %d  ", g->snake.body[1].tiley);
    }
    mvwprintw(win, y + 2, 15, "Fruits : %d  ", fEats);
  }

  wrefresh(win);
  return;
}

void tileUpdate(Game *g, Tile t, Cords old, Cords new) {
  short x = g->width;

  int tempOldInd = old.tiley * x + old.tilex;
  int tempNewInd = new.tiley * x + new.tilex;

  if (t != SPACE) {
    g->tiles[tempOldInd].tileType = SPACE;
  }

  g->tiles[tempNewInd].tileType = t;
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

  tileUpdate(g, HEAD, *old, (Cords){newx, newy});

  s->headPos.tilex = newx;
  s->headPos.tiley = newy;

  oldx = s->body[0].tilex;
  oldy = s->body[0].tiley;

  s->body[0].tilex = newx;
  s->body[0].tiley = newy;

  for (char i = 1; i < s->length; i++) {

    short tempx = s->body[i].tilex;
    short tempy = s->body[i].tiley;
    tileUpdate(g, SNAKE, (Cords){tempx, tempy}, (Cords){oldx, oldy});

    s->body[i].tilex = oldx;
    s->body[i].tiley = oldy;

    oldx = tempx;
    oldy = tempy;
  }

  return;
}

void snakeVision(Game *g, Snake *s) {
  Cords curr = s->headPos;

  for (char x = 0; x < s->visionL; x++) {
  }

  return;
}

void snakeUpdate(Game *g, Snake *s) {
  short x = g->width;
  short y = g->height;

  Cords *fruit = &g->fruit.pos;
  Cords snake = s->headPos;

  if (snake.tiley < fruit->tiley) {
    // Down
    s->direction = TOP;
    snakeMoveUpdate(g, &snake, s);
  } else if (snake.tiley > fruit->tiley) {
    // up
    s->direction = BOTTOM;
    snakeMoveUpdate(g, &snake, s);
  } else {
    if (snake.tilex < fruit->tilex) {
      // right
      s->direction = LEFT;
      snakeMoveUpdate(g, &snake, s);
    }
    if (snake.tilex > fruit->tilex) {

      // left
      s->direction = RIGHT;
      snakeMoveUpdate(g, &snake, s);
    }
  }

  snake = s->headPos;
  if (fruit->tiley == snake.tiley && fruit->tilex == snake.tilex) {
    if (s->length < g->snakeCap) {
      s->length++;
    }

    fEats++;
    fruitInit(g);
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

int main() {
  short y, x;
  Game game;

  srand(time(NULL));

  // initial ncurses stuff
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  // colors
  start_color();

  init_pair(1, COLOR_GREEN, COLOR_WHITE); // bg
  init_pair(2, COLOR_GREEN, COLOR_RED);   // fruit
  init_pair(4, COLOR_GREEN, COLOR_BLACK); // snake
  init_pair(3, COLOR_GREEN, COLOR_GREEN); // snake

  WINDOW *win = newwin(LINES, COLS, 0, 0);
  getmaxyx(win, y, x);
  y = y - 10;

  box(win, 0, 0);
  gameInit(&game, x, y);

  // increase to increase the number of snakes
  snakeInit(&game);
  snakeInit(&game);

  fruitInit(&game);

  gameRender(&game, win);

  wtimeout(win, 17);

  while (1) {
    int ch = wgetch(win);
    gameUpdate(&game, win);
    if (ch == 'q')
      break;
    if (ch == 'r') {
      fruitInit(&game);
      gameRender(&game, win);
    }
  }
  free(game.tiles);
  for (char i = 0; i < game.snakeCnt; i++) {
    free(game.snake[i].body);
  }
  free(game.snake);

  endwin();
  return 0;
}
