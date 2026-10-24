#include <stdio.h>
#include "mapMake.h"

int mapMake(char map[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
                map[i][j] = '\0';        
        }
    }
    return 0;
}

int printMapScan(char map[HEIGHT][WIDTH], VehPosType vehPos, int posX, int posY, char value) {
    printf("\n================================\n");
    printf("          SCANNED MAP             \n");
    printf("================================\n\n");
    if(value != '\0') {
        map[vehPos.y - posY][vehPos.x + posX] = value;        
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == vehPos.y && j == vehPos.x) {
                putchar('_');
            } else if (map[i][j] == '\0') {
                putchar(' ');
            } else {
                putchar(map[i][j]);
            }
            putchar(' '); 
        }
        putchar('\n');
    }
    return 0;
}

int printMapExplore(char map[HEIGHT][WIDTH], VehPosType vehPos, char value) {
    printf("\n================================\n");
    printf("          EXPLORED MAP          \n");
    printf("================================\n\n");
    map[vehPos.y][vehPos.x] = value;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if(i == vehPos.y && j == vehPos.x && value == '\0') {
                printf("\033[4m \033[0m");
            }
            else if (i == vehPos.y && j == vehPos.x) {
                printf("\033[4m%c\033[0m", value);
            } else if (map[i][j] == '\0') {
                putchar(' ');
            } else if(map[i][j] == ' ') {
                printf("\033[47m%c\033[0m", map[i][j]);
            } else {
                putchar(map[i][j]);
            }
            putchar(' '); 
        }
        putchar('\n');
    }
    return 0;
}

void printMapStatic(char map[HEIGHT][WIDTH], VehPosType vehPos) {
    printf("\n================================\n");
    printf("          EXPLORED MAP          \n");
    printf("================================\n\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if(map[i][j] == '\0' && i == vehPos.y && j == vehPos.x) {
                printf("\033[4m \033[0m");
            } else if (map[i][j] == '\0') {
                putchar(' ');
            } else if(i == vehPos.y && j == vehPos.x) {
                printf("\033[4m%c\033[0m", map[vehPos.y][vehPos.x]);
            } else if(map[i][j] == ' ') {
                printf("\033[47m%c\033[0m", map[i][j]);
            } else {
                putchar(map[i][j]);
            }
            putchar(' '); 
        }
        putchar('\n');
    }
}