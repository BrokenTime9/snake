#!/bin/bash

gcc src/*.c src/snake/*.c util/util.c -lncurses -o game
./game
