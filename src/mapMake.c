#include <stdio.h>
#include <stdlib.h>
// #include "config.h"

#define HEIGHT 7
#define WIDTH 25

typedef struct {
    int x;
    int y;
} VehPosType;

int mapMake(char map[HEIGHT][WIDTH], VehPosType vehPos) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            map[i][j] = '.';        //change to spacebar isntead of . later
        }
    }
    map[vehPos.y][vehPos.x] = '_';
    return 0;
}

int printMapScan(char map[HEIGHT][WIDTH], VehPosType vehPos, int posX, int posY, char value) {
    printf("================================\n");
    printf("          SCANNED MAP            \n");
    printf("================================\n\n");
    map[vehPos.y + posY][vehPos.x + posX] = value;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            putchar(map[i][j]);
            putchar(' '); 
        }
        putchar('\n');
    }
    return 0;
}

int printMapExplore(char map[HEIGHT][WIDTH], VehPosType *vehPos, int posX, int posY, char value) {
    printf("================================\n");
    printf("          EXPLORED MAP          \n");
    printf("================================\n\n");
    map[(*vehPos).y][(*vehPos).x] = '.';  //delete OLD
    map[(*vehPos).y + posY][(*vehPos).x + posX] = '_';  //create NEW

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            putchar(map[i][j]);
            putchar(' '); 
        }
        putchar('\n');
    }
    return 0;
}

int main() {
    char scanMap[HEIGHT][WIDTH];
    char exploreMap[HEIGHT][WIDTH];
    VehPosType vehPos;
    vehPos.x = WIDTH/2;
    vehPos.y = HEIGHT/2;

    mapMake(scanMap, vehPos);
    mapMake(exploreMap, vehPos);
    printMapScan(scanMap, vehPos, 1, 0, 'M');
    printMapScan(scanMap, vehPos, -1, 0, 'V');
    printf("\n");
    printMapExplore(exploreMap, &vehPos, -1, 0, 'V');
}