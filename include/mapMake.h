#ifndef MAP_H
#define MAP_H

#include "config.h"

typedef struct {
    int x;
    int y;
} VehPosType;

int mapMake(char map[HEIGHT][WIDTH]);

int printMapScan(char map[HEIGHT][WIDTH],
                VehPosType vehPos,
                int posX,
                int posY,
                char value);

int printMapExplore(char map[HEIGHT][WIDTH],
                    VehPosType vehPos,
                    char value);

void printMapStatic(char map[HEIGHT][WIDTH],
                    VehPosType vehPos);

#endif
