#!/bin/bash

gcc src/init.c src/render.c src/main.c src/update.c -lncurses -o game
./game
