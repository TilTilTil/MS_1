#ifndef MAP_H
#define MAP_H

#include "config.h"

typedef struct {
    int x;
    int y;
} PosType;

void mapMake(char map[HEIGHT][WIDTH]);

void printMapScan(char map[HEIGHT][WIDTH],
                PosType vehPos,
                PosType localPos,
                char value);

void printMapExplore(char map[HEIGHT][WIDTH],
                    PosType vehPos,
                    char value);

void printMapStatic(char map[HEIGHT][WIDTH],
                    PosType vehPos);

#endif
