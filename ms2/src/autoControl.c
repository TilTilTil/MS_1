#include <stdio.h>
#include "vehControl.h"
#include "SatComRelay.h"
#include "mapMake.h"
#include "vehControl.h"
#include "errorHandle.h"
#include <ctype.h>
#include <time.h>

#define STARTSHIELD 7000

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
    for (int i = 0; i < sizeof(dirX); i++) {
        localX = dirX[i];
        localY = dirY[i];
        scanHelper(vehicleData, vehPos, scanMap, true, localX, localY, Counter);
        scanHelper(vehicleData, vehPos, scanMap, false, localX, localY, Counter);
    }
}


static void moveScanDir(VehDataType *vehicleData, VehPosType vehPos, char scanMap[HEIGHT][WIDTH], int *Counter, char dir) {
    int localX;
    int localY;
    int dirX[5];
    int dirY[5];
    switch (dir) {
        case 'N':
            dirX[0] =  0; dirY[0] = -1;
            dirX[1] = -1; dirY[1] = -1;
            dirX[2] =  1; dirY[2] = -1;
            dirX[3] = -1; dirY[3] =  0;
            dirX[4] =  1; dirY[4] =  0;
            break;
        case 'S':
            dirX[0] =  0; dirY[0] =  1;
            dirX[1] = -1; dirY[1] =  1;
            dirX[2] =  1; dirY[2] =  1;
            dirX[3] = -1; dirY[3] =  0;
            dirX[4] =  1; dirY[4] =  0;
            break;
        case 'E':
            dirX[0] =  1; dirY[0] =  0;
            dirX[1] =  1; dirY[1] = -1;
            dirX[2] =  1; dirY[2] =  1;
            dirX[3] =  0; dirY[3] = -1;
            dirX[4] =  0; dirY[4] =  1;
            break;
        case 'W':
            dirX[0] = -1; dirY[0] =  0;
            dirX[1] = -1; dirY[1] = -1;
            dirX[2] = -1; dirY[2] =  1;
            dirX[3] =  0; dirY[3] = -1;
            dirX[4] =  0; dirY[4] =  1;
            break;
        default:
            return;
    }
    for (int i = 0; i < 5; i++) {
        localX = dirX[i];
        localY = dirY[i];
        scanHelper(vehicleData, vehPos, scanMap, true, localX, localY, Counter);
        scanHelper(vehicleData, vehPos, scanMap, false, localX, localY, Counter);
    }
}


static void boundHandle(terrainCostType terrainCostLog[256], VehPosType *vehPos, char scanMap[HEIGHT][WIDTH]){
    //should be scanned already. //remake scanHelper so it does not scan an area that is already scanned
    int costLeft = scanMap[vehPos->x][vehPos->y];
    int costRight = scanMap[vehPos->x][vehPos->y];
}

void autoRun() {
    //roadmap design
}