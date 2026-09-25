#ifndef MAP_H
#define MAP_H

#define HEIGHT 20
#define WIDTH 50

typedef struct {
    int x;
    int y;
} VehPosType;

typedef struct {
    int x;
    int y;
    char value;
} LocalTerrainType;

int mapMake(char map[HEIGHT][WIDTH], 
            VehPosType vehPos);

int printMapScan(char map[HEIGHT][WIDTH],
                 VehPosType vehPos,
                 int posX,
                 int posY,
                 char value);

int printMapExplore(char map[HEIGHT][WIDTH],
                    VehPosType *vehPos,
                    int posX,
                    int posY);

#endif
