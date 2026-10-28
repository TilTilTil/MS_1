#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

#include "vehControl.h"
#include "SatComRelay.h"
#include "mapMake.h"
#include "vehControl.h"
#include "errorHandle.h"
#include "autoControl.h"

#define STARTSHIELD 70000

// static void delay(long number_of_seconds)
// {
//     // Storing start time
//     clock_t start_time = clock();

//     // Looping till required time is not achieved
//     while (clock() - start_time < number_of_seconds * CLOCKS_PER_SEC)
//         ;
// }

// static void scanDelay(VehDataType *vehicleData, VehPosType vehPos, char scanMap[HEIGHT][WIDTH], bool radiusBoolOne, int localX, int localY, int *Counter, long *time) {
//     scanHelper(vehicleData, vehPos, scanMap, radiusBoolOne, localX, localY, Counter);
//     delay(time);
// }

// static void moveDelay(VehDataType *vehicleData, VehPosType *vehPos, char scanMap[HEIGHT][WIDTH], char exploreMap[HEIGHT][WIDTH], int localX, int localY, int *Counter, long *time) {
//     moveHelper(vehicleData, vehPos, scanMap, exploreMap, localX, localY, Counter);
//     delay(time);
// }

void autoConfig() {
    bool opt1 = true, opt2 = true, opt3 = true, opt4 = true, opt5 = true, opt6 = true;
    long opt7,opt8,opt9,opt10,opt11,opt12,opt13;

    printf("--- Vehicle Configuration Menu ---\n\n");

    // Option chuck 1
    if(getValidChar("\n\n1. After every action, show vehicle status / energy stats (y/n): \n", "YN") == 'Y') {
        opt1 = true;
    } else {
        if(getValidChar("\n2. After every scan action, show vehicle status / energy stats (y/n): \n", "YN") == 'N') {
            opt2 = false;
        }
        if(getValidChar("\n3. After every move action, show vehicle status / energy stats (y/n): \n", "YN") == 'N') {
            opt3 = false;
        }
        if(getValidChar("\n4. After every shield allocation, show vehicle status / energy stats (y/n): \n", "YN") == 'N') {
            opt4 = false;
        }
    }
    // option chuck 2
    if(getValidChar("\n5. After every scan action, display 'current map' of terrain scanned by vehicle so far (y/n): \n", "YN") == 'N') {
        opt5 = false;
    }
    if(getValidChar("\n6. After every move action, display 'current map' of terrain travelled by vehicle so far (y/n): \n", "YN") == 'N') {
        opt6 = false;
    }
    printf("\n7. Enter 't1' secs delay, after displaying vehicle status / energy stats: \n");
    opt7 = getValidLongHelper();
    printf("\n8. Enter 't2' secs delay, after displaying 'current map' of terrain scanned: \n");
    opt8 = getValidLongHelper();
    printf("\n9. Enter 't3' secs delay, after displaying 'current map' of terrain travelled: \n");
    opt9 = getValidLongHelper();

    //option chuck 3
    printf("10. Enter 't4' secs delay, after any action taken by auto-pilot function: \n");
    opt10 = getValidLongHelper();
    if(opt10 > 0) {
        opt11 = opt12 = opt13 = opt10;
    } else {
        printf("\n11. Enter 't5' secs delay, after each scan action taken by auto-pilot function: \n");
        opt11 = getValidLongHelper();
        printf("\n12. Enter 't6' secs delay, after each move taken by auto-pilot function: \n");
        opt12 = getValidLongHelper();
        printf("\n13. Enter 't7' secs delay, after each shield allocation action taken by auto-pilot function: \n");
        opt13 = getValidLongHelper();
    }
    printf("\n--- Configuration Summary ---\n");
    printf("Option 1 (All Status): %d\n", opt1);
    printf("Option 2 (Scan Status): %d\n", opt2);
    printf("Option 3 (Move Status): %d\n", opt3);
    printf("Option 4 (Shield Status): %d\n", opt4);
    printf("Option 5 (Scan Map): %d\n", opt5);
    printf("Option 6 (Travel Map): %d\n", opt6);
    printf("Option 7 (Status Display Delay): %ld seconds\n", opt7);
    printf("Option 8 (Scanned Map Display Delay): %ld seconds\n", opt8);
    printf("Option 9 (Travelled Map Display Delay): %ld seconds\n", opt9);
    printf("Option 10 (Auto-Pilot Action Delay): %ld seconds\n", opt10);
    printf("Option 11 (Auto-Pilot Scan Delay): %ld seconds\n", opt11);
    printf("Option 12 (Auto-Pilot Move Delay): %ld seconds\n", opt12);
    printf("Option 13 (Auto-Pilot Shield Allocation Delay): %ld seconds\n", opt13);
}

static void initAuto(VehDataType *vehicleData, VehPosType vehPos, char scanMap[HEIGHT][WIDTH], int *Counter) {
    //start with full scan then allocation of shield
    *vehicleData = SCRelay_Signal_Veh_AllocateToShield(STARTSHIELD);
    int dirX[8] = { 0,  1,  1,  1,  0, -1, -1, -1};
    int dirY[8] = {-1, -1,  0,  1,  1,  1,  0, -1};
    int localX;
    int localY;
    for (int i = 0; i < sizeof(dirX) / sizeof(dirX[0]); i++) {
        localX = dirX[i];
        localY = dirY[i];
        scanHelper(vehicleData, vehPos, scanMap, true, localX, localY, Counter);
        scanHelper(vehicleData, vehPos, scanMap, false, localX, localY, Counter);
    }
}

static void moveScanVectorUnit(VehDataType *vehicleData, VehPosType *vehPos, terrainCostType terrainCostLog[256], char scanMap[HEIGHT][WIDTH], char exploreMap[HEIGHT][WIDTH], int *scanCounter, int *moveCounter, char *vehicleFacingDir, char dir) {
    int localX;
    int localY;
    int semiCircleX[5];
    int semiCircleY[5];
    switch (dir) {
        case 'N':
            semiCircleX[0] =  0; semiCircleY[0] = 1;
            semiCircleX[1] = -1; semiCircleY[1] = 1;
            semiCircleX[2] =  1; semiCircleY[2] = 1;
            semiCircleX[3] = -1; semiCircleY[3] =  0;
            semiCircleX[4] =  1; semiCircleY[4] =  0;
            moveHelper(vehicleData, vehPos ,terrainCostLog ,scanMap, exploreMap, 0, 1, moveCounter);
            *vehicleFacingDir = dir;
            break;
        case 'S':
            semiCircleX[0] =  0; semiCircleY[0] =  -1;
            semiCircleX[1] = -1; semiCircleY[1] =  -1;
            semiCircleX[2] =  1; semiCircleY[2] =  -1;
            semiCircleX[3] = -1; semiCircleY[3] =  0;
            semiCircleX[4] =  1; semiCircleY[4] =  0;
            moveHelper(vehicleData, vehPos ,terrainCostLog ,scanMap, exploreMap, 0, -1, moveCounter);
            *vehicleFacingDir = dir;
            break;
        case 'E':
            semiCircleX[0] =  1; semiCircleY[0] =  0;
            semiCircleX[1] =  1; semiCircleY[1] = -1;
            semiCircleX[2] =  1; semiCircleY[2] =  1;
            semiCircleX[3] =  0; semiCircleY[3] = -1;
            semiCircleX[4] =  0; semiCircleY[4] =  1;
            moveHelper(vehicleData, vehPos ,terrainCostLog ,scanMap, exploreMap, 1, 0, moveCounter);
            *vehicleFacingDir = dir;
            break;
        case 'W':
            semiCircleX[0] = -1; semiCircleY[0] =  0;
            semiCircleX[1] = -1; semiCircleY[1] = -1;
            semiCircleX[2] = -1; semiCircleY[2] =  1;
            semiCircleX[3] =  0; semiCircleY[3] = -1;
            semiCircleX[4] =  0; semiCircleY[4] =  1;
            moveHelper(vehicleData, vehPos ,terrainCostLog ,scanMap, exploreMap, -1, 0, moveCounter);
            *vehicleFacingDir = dir;
            break;
        default:
            return;
    }
    for (int i = 0; i < 5; i++) {
        localX = semiCircleX[i];
        localY = semiCircleY[i];
        scanHelper(vehicleData, *vehPos, scanMap, true, localX, localY, scanCounter);
        scanHelper(vehicleData, *vehPos, scanMap, false, localX, localY, scanCounter);
    }
}

static void boundHandle(terrainCostType terrainCostLog[256], VehPosType *vehPos, char scanMap[HEIGHT][WIDTH], char *vehicleFacingDir){
    int costLeft;
    int costRight;
    switch(*vehicleFacingDir) {
        case 'N':
            if(scanMap[vehPos->y][vehPos->x - 1] == '#' || scanMap[vehPos->y][vehPos->x -2] == '#') {
                *vehicleFacingDir = 'E';
            } else if(scanMap[vehPos->y][vehPos->x + 1] == '#' || scanMap[vehPos->y][vehPos->x + 2] == '#') {
                *vehicleFacingDir = 'W';
            } else {
                costLeft = terrainCostLog[scanMap[vehPos->y][vehPos->x - 1]].totalCost + terrainCostLog[scanMap[vehPos->y][vehPos->x - 2]].totalCost;
                costRight = terrainCostLog[scanMap[vehPos->y][vehPos->x + 1]].totalCost + terrainCostLog[scanMap[vehPos->y][vehPos->x + 2]].totalCost;
                (costLeft > costRight) ? (*vehicleFacingDir = 'E') : (*vehicleFacingDir = 'W');
            }
            break;
        case 'S':
            if(scanMap[vehPos->y][vehPos->x + 1] == '#' || scanMap[vehPos->y][vehPos->x + 2] == '#') {
                *vehicleFacingDir = 'W';
            } else if(scanMap[vehPos->y][vehPos->x - 1] == '#' || scanMap[vehPos->y][vehPos->x - 2] == '#') {
                *vehicleFacingDir = 'E';
            } else {
                costLeft = terrainCostLog[scanMap[vehPos->y][vehPos->x + 1]].totalCost + terrainCostLog[scanMap[vehPos->y][vehPos->x + 2]].totalCost;
                costRight = terrainCostLog[scanMap[vehPos->y][vehPos->x - 1]].totalCost + terrainCostLog[scanMap[vehPos->y][vehPos->x - 2]].totalCost;
                (costLeft > costRight) ? (*vehicleFacingDir = 'W') : (*vehicleFacingDir = 'E');
            }
            break;
        case 'E':
            if(scanMap[vehPos->y - 1][vehPos->x] == '#' || scanMap[vehPos->y - 2][vehPos->x] == '#') {
                *vehicleFacingDir = 'S';
            } else if(scanMap[vehPos->y + 1][vehPos->x] == '#' || scanMap[vehPos->y + 2][vehPos->x] == '#') {
                *vehicleFacingDir = 'N';
            } else {
                costLeft = terrainCostLog[scanMap[vehPos->y - 1][vehPos->x]].totalCost + terrainCostLog[scanMap[vehPos->y - 2][vehPos->x]].totalCost;
                costRight = terrainCostLog[scanMap[vehPos->y + 1][vehPos->x]].totalCost + terrainCostLog[scanMap[vehPos->y + 2][vehPos->x]].totalCost;
                (costLeft > costRight) ? (*vehicleFacingDir = 'S') : (*vehicleFacingDir = 'N');
            }
            break;
        case 'W':
            if(scanMap[vehPos->y + 1][vehPos->x] == '#' || scanMap[vehPos->y + 2][vehPos->x] == '#') {
                *vehicleFacingDir = 'N';
            } else if(scanMap[vehPos->y - 1][vehPos->x] == '#' || scanMap[vehPos->y - 2][vehPos->x] == '#') {
                *vehicleFacingDir = 'S';
            } else {
                costLeft = terrainCostLog[scanMap[vehPos->y + 1][vehPos->x]].totalCost + terrainCostLog[scanMap[vehPos->y + 2][vehPos->x]].totalCost;
                costRight = terrainCostLog[scanMap[vehPos->y - 1][vehPos->x]].totalCost + terrainCostLog[scanMap[vehPos->y - 2][vehPos->x]].totalCost;
                (costLeft > costRight) ? (*vehicleFacingDir = 'N') : (*vehicleFacingDir = 'S');
            }
            break;
        default:
            return;
    }
}

static char moveScanMode(VehDataType *vehicleData, VehPosType *vehPos, terrainCostType terrainCostLog[256], char scanMap[HEIGHT][WIDTH], char exploreMap[HEIGHT][WIDTH], int *scanCounter, int *moveCounter, char vehicleFacingDir, int step) {
    switch(vehicleFacingDir) {
        case 'N':
            if(step == 0) {
                while(scanMap[vehPos->y - 1][vehPos->x] != '#' && scanMap[vehPos->y - 2][vehPos->x] != '#') {
                    moveScanVectorUnit(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, &vehicleFacingDir, 'N');
                }
                //boundary logger here
                moveScanVectorUnit(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, &vehicleFacingDir, 'S');
                boundHandle(terrainCostLog, vehPos, scanMap, &vehicleFacingDir);
            } else {
                for(int i = 0; i < step; i++) {
                    if(scanMap[vehPos->y - 2][vehPos->x] == '#') {
                        moveScanVectorUnit(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, &vehicleFacingDir, 'S');
                        break;
                    }
                    moveScanVectorUnit(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, &vehicleFacingDir, 'N');
                }
            }
            break;
        case 'S':
            if(step == 0) {
                while(scanMap[vehPos->y + 1][vehPos->x] != '#' && scanMap[vehPos->y + 2][vehPos->x] != '#') {
                    moveScanVectorUnit(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, &vehicleFacingDir, 'S');
                }
                moveScanVectorUnit(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, &vehicleFacingDir, 'N');
                boundHandle(terrainCostLog, vehPos, scanMap, &vehicleFacingDir);
            } else {
                for(int i = 0; i < step; i++) {
                    if(scanMap[vehPos->y + 2][vehPos->x] == '#') {
                        moveScanVectorUnit(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, &vehicleFacingDir, 'N');
                        break;
                    }
                    moveScanVectorUnit(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, &vehicleFacingDir, 'S');
                }
            }
            break;
        case 'E':
            if(step == 0) {
                while(scanMap[vehPos->y][vehPos->x + 1] != '#' && scanMap[vehPos->y][vehPos->x + 2] != '#') {
                    moveScanVectorUnit(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, &vehicleFacingDir, 'E');
                }
                moveScanVectorUnit(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, &vehicleFacingDir, 'W');
                boundHandle(terrainCostLog, vehPos, scanMap, &vehicleFacingDir);
            } else {
                for(int i = 0; i < step; i++) {
                    if(scanMap[vehPos->y][vehPos->x + 2] == '#') {
                        moveScanVectorUnit(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, &vehicleFacingDir, 'W');
                        break;
                    }
                    moveScanVectorUnit(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, &vehicleFacingDir, 'E');
                }
            }
            break;
        case 'W':
            if(step == 0) {
                while(scanMap[vehPos->y][vehPos->x - 1] != '#' && scanMap[vehPos->y][vehPos->x - 2] != '#') {
                    moveScanVectorUnit(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, &vehicleFacingDir, 'W');
                }
                moveScanVectorUnit(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, &vehicleFacingDir, 'E');
                boundHandle(terrainCostLog, vehPos, scanMap, &vehicleFacingDir);
            } else {
                for(int i = 0; i < step; i++) {
                    if(scanMap[vehPos->y][vehPos->x - 2] == '#') {
                        moveScanVectorUnit(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, &vehicleFacingDir, 'E');
                        break;
                    }
                    moveScanVectorUnit(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, &vehicleFacingDir, 'W');
                }
            }
            break;
        default:
            return vehicleFacingDir;
    }
    return vehicleFacingDir;
}

void autoRun(VehDataType *vehicleData, VehPosType *vehPos, terrainCostType terrainCostLog[256], char scanMap[HEIGHT][WIDTH], char exploreMap[HEIGHT][WIDTH], int *scanCounter, int *moveCounter) {
    char vehicleFacingDir = 'N';    //for now hard coded but later make into arg
    char vehicleOppStart;
    switch(vehicleFacingDir) {
        case 'N': vehicleOppStart = 'S'; break;
        case 'S': vehicleOppStart = 'N'; break;
        case 'E': vehicleOppStart = 'W'; break;
        case 'W': vehicleOppStart = 'E'; break;
        default:  return;
    }

    initAuto(vehicleData, *vehPos, scanMap, scanCounter);
    vehicleFacingDir = moveScanMode(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, vehicleFacingDir, 0); //hit north boundary
    int topBound = vehPos->y;
    vehicleFacingDir = moveScanMode(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, vehicleOppStart, 0); //hit South boundary hard forced to be opposite of starting Dir
    int bottomBound = vehPos->y;
    int middleBound = vehPos->x;
    vehicleFacingDir = moveScanMode(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, vehicleFacingDir, 0); //either west or east boundary
    int thirdBound = vehPos->x;
    vehicleFacingDir = moveScanMode(vehicleData, vehPos, terrainCostLog, scanMap, exploreMap, scanCounter, moveCounter, vehicleFacingDir, abs(topBound - bottomBound)); //either west or east boundary
    printf("%d",abs(topBound - bottomBound));
}