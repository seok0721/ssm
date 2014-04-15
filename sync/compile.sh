#!/bin/sh
rm -f trade_yield
rm -f trade_selfish
rm -f economy

gcc -pthread -o trade_yield trade_yield.c
gcc -pthread -o trade_selfish trade_selfish.c
gcc -pthread -o economy economy.c
