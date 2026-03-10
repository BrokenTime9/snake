#include "../include/game.h"

Cords add(short x, short y, Cords dir, short skip) {
  short retx = x + (dir.tilex * skip);
  short rety = y + (dir.tiley * skip);
  return (Cords){retx, rety};
};

bool isIn(Cords x, Cords *y, short l) {
  bool b = false;
  for (int i = 0; i < l; i++) {
    if (x.tilex == y[i].tilex && x.tiley == y[i].tiley) {
      b = true;
    }
  }

  return b;
}
