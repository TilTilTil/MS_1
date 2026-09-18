#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define HEIGHT 10
#define WIDTH 30

typedef struct {
    int x;
    int y;
} VehPosType;

typedef struct {
    int x;
    int y;
    char value;
} LocalTerrainType;

// #include "SatComRelay.h"
// #include "mapMake.h"

// VehDataType initVehicle() {
//     return SCRelay_Signal_Veh_Init(
//         "scenario/scenario.dat",
//         'y',
//         'y',
//         '8'
//     );
// }

int mapMake(char map[HEIGHT][WIDTH], VehPosType vehPos) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            map[i][j] = '.';        //change to spacebar isntead of . later
        }
    }
    // map[vehPos.y][vehPos.x] = '_';
    return 0;
}

int printMapScan(char map[HEIGHT][WIDTH], VehPosType vehPos, LocalTerrainType Terrain) {
    printf("\n================================\n");
    printf("          SCANNED MAP            \n");
    printf("================================\n\n");
    // map[(vehPos).y][(vehPos).x] = '_'; 
    map[vehPos.y - Terrain.y][vehPos.x - Terrain.x] = Terrain.value;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if(i == vehPos.y && j == vehPos.x) {
                putchar('_');
            }
            else 
                putchar(map[i][j]);
            putchar(' '); 
        }
        putchar('\n');
    }
    // map[(vehPos).y][(vehPos).x] = '.'; 
    return 0;
}

// int printMapExplore(char map[HEIGHT][WIDTH], VehPosType *vehPos, int posX, int posY) {
//     printf("\n================================\n");
//     printf("          EXPLORED MAP          \n");
//     printf("================================\n\n");
//     map[(*vehPos).y][(*vehPos).x] = '.';  //delete OLD
//     map[(*vehPos).y - posY][(*vehPos).x - posX] = '_';  //create NEW

//     for (int i = 0; i < HEIGHT; i++) {
//         for (int j = 0; j < WIDTH; j++) {
//             putchar(map[i][j]);
//             putchar(' '); 
//         }
//         putchar('\n');
//     }
//     return 0;
// }

int main() {
    
    // VehDataType vehicleData = initVehicle();
    // SCRelay_Signal_Veh_Init(
    //     "scenario/scenario.dat",
    //     'y',
    //     'y',
    //     '8'
    // );
    // VehDataType Veh;
    // Veh = SCRelay_Signal_Veh_AllocateToShield(200000);
    // printf("Initial Energy       : %ld\n", Veh.initialEnergy);
    // printf("Current Energy       : %ld\n", Veh.currEnergy);
    // printf("Current Shield Energy: %ld\n", Veh.currShieldEnergy);
    // printf("Total Energy Used    : %ld\n", Veh.totEnergyUsed);
    // printf("Total Shield Used    : %ld\n", Veh.totShieldEnergyUsed);
    // printf("\n");
    // Veh = SCRelay_Signal_Veh_MoveUpNorth();
    // printf("Initial Energy       : %ld\n", Veh.initialEnergy);
    // printf("Current Energy       : %ld\n", Veh.currEnergy);
    // printf("Current Shield Energy: %ld\n", Veh.currShieldEnergy);
    // printf("Total Energy Used    : %ld\n", Veh.totEnergyUsed);
    // printf("Total Shield Used    : %ld\n", Veh.totShieldEnergyUsed);

    // char scanMap[HEIGHT][WIDTH];
    // char scanMap1[HEIGHT][WIDTH];
    // char exploreMap[HEIGHT][WIDTH];
    // VehPosType vehPos;
    // LocalTerrainType Terrain;
    // vehPos.x = WIDTH/2;
    // vehPos.y = HEIGHT/2;
    // Terrain.x = 1;
    // Terrain.y = 1;
    // Terrain.value = 'M';
    // mapMake(scanMap, vehPos);
    // mapMake(scanMap1, vehPos);

    // printMapScan(scanMap, vehPos, Terrain);
    // vehPos.x = WIDTH/2;
    // vehPos.y = HEIGHT/2 -1;
    // Terrain.x = -1;
    // Terrain.y = 0;
    // Terrain.value = 'M';
    // printMapScan(scanMap, vehPos, Terrain);

    SCRelay_Signal_Veh_MoveUpNorth();

    // vehPos.x = WIDTH/2;
    // vehPos.y = HEIGHT/2;
    // printf("TEST");
    // printf("%d",vehPos.x);
    // vehPos.x = vehPos.x + 1;
    // printf("%d",vehPos.x);
}



// Two ways to approach this, i could create a 3D Array Map or i could create a struct array.
// issue now is in going thru the object "deletes it"
// 3D array map is so that i can have two maps in one array. The second layer will solely be for the Terrain objects, The first will be the one i printf from.
// it will duplicate layer two into layer one. This is so that whereever the car moves, any object will be deleted in layer one but not on layer two.

//Struct array could be used by saving the exact position of the structure and then duplicated every single time the map array is called.
//This is so as usualy whenever the car moves, the struct array will have all the position of terrain noted down and hence cant be deleted

//lol got simpler method ahahahah
