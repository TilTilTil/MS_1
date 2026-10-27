#ifndef MAP_H
#define MAP_H

#include "config.h"

typedef struct {
    int x;
    int y;
} VehPosType;

void mapMake(char map[HEIGHT][WIDTH]);

void printMapScan(char map[HEIGHT][WIDTH],
                VehPosType vehPos,
                int posX,
                int posY,
                char value);

void printMapExplore(char map[HEIGHT][WIDTH],
                    VehPosType vehPos,
                    char value);

void printMapStatic(char map[HEIGHT][WIDTH],
                    VehPosType vehPos);

#endif
