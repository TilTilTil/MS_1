#ifndef MAP_H
#define MAP_H

#include "config.h"
#include "helpers.h"

typedef struct {
    int x;
    int y;
} PosType;

void mapMake(char map[HEIGHT][WIDTH]);

void printMapScan(char map[HEIGHT][WIDTH],
                PosType vehPos,
                PosType localPos,
                Option configOption,
                char value);

void printMapExplore(char map[HEIGHT][WIDTH],
                    PosType vehPos,
                    Option configOption,
                    char value);

void printMapStatic(char map[HEIGHT][WIDTH],
                    PosType vehPos);

#endif
