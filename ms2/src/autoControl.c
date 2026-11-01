#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

#include "vehControl.h"
#include "SatComRelay.h"
#include "mapMake.h"
#include "helpers.h"
#include "autoControl.h"
#include "config.h"
#include "condtionHandle.h"
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

static DirectionType turnClockwise(DirectionType dir, int degrees) {
    return (dir + degrees / 90) % 4;
}

static DirectionType turnAntiClockwise(DirectionType dir, int degrees) {
    return (dir - degrees / 90 + 4) % 4;
}

static PosType rightOfPos(PosType pos) {
    return (PosType){
        .x = -pos.y,
        .y = pos.x
    };
}

static PosType leftOfPos(PosType pos) {
    return (PosType){
        .x = pos.y,
        .y = -pos.x
    };
}

static bool isWall(char c) {
    return c == '#' || c == FILL;
}

static PosType dirToAxis(DirectionType dir) {
    switch (dir) {
        case DIR_NORTH: return NORTH;
        case DIR_EAST:  return EAST;
        case DIR_SOUTH: return SOUTH;
        case DIR_WEST:  return WEST;
        default:        return NORTH;
    }
}

static void initAuto(AutoState *a) {
    *a->veh = SCRelay_Signal_Veh_AllocateToShield(START_SHIELD);
    for (int i = 0; i < sizeof(ALLAXIS) / sizeof(ALLAXIS[0]); i++) {
        if(a->configOption.opt11 > 0){delay(a->configOption.opt11);}
        scanHelper(a->veh, *a->vehPos, ALLAXIS[i], a->scanMap, true, a->scanCounter, a->configOption);
        if (shouldStop(a)) return;
        if(a->configOption.opt11 > 0){delay(a->configOption.opt11);}
        scanHelper(a->veh, *a->vehPos, ALLAXIS[i], a->scanMap, false, a->scanCounter, a->configOption);
        if(shouldStop(a)) {return;};
    }
    tryVisitE(a);
    shouldStop(a);
}

static void fillOutside(char scanMap[HEIGHT][WIDTH], int wx, int wy, PosType axis, bool flush) {
    int yStart = 0, yEnd = HEIGHT - 1, xStart = 0, xEnd = WIDTH - 1;

    if (axis.y < 0 && flush)       { yEnd = wy - 1; } 
    else if (axis.y > 0 && flush)  { yStart = wy + 1; }  
    else if (axis.x > 0 && flush)  { xStart = wx + 1; }  
    else if (axis.x < 0 && flush)  { xEnd = wx - 1; }   
    else if (axis.y < 0 && !flush) { yEnd = wy; }     
    else if (axis.y > 0 && !flush) { yStart = wy; }   
    else if (axis.x > 0 && !flush) { xStart = wx; }     
    else if (axis.x < 0 && !flush) { xEnd = wx; }       

    for (int y = yStart; y <= yEnd; y++) {
        for (int x = xStart; x <= xEnd; x++) {
            if (scanMap[y][x] != '#') {
                scanMap[y][x] = FILL;
            }
        }
    }
}

static void stepAndScan(AutoState *a) {
    PosType axis = dirToAxis(a->facing);
    char inFront = a->scanMap[a->vehPos->y + axis.y][a->vehPos->x + axis.x];
    if (a->GGNOOBS) return;
    if(a->configOption.opt13 > 0){delay(a->configOption.opt13);}
    *a->veh = SCRelay_Signal_Veh_AllocateToShield(a->costLog[inFront].shield);
    if(a->configOption.opt12 > 0){delay(a->configOption.opt12);}
    moveHelper(a->veh, a->vehPos, axis, a->costLog, a->scanMap, a->exploreMap, a->moveCounter, a->configOption);

    for (int i = 0; i < sizeof(ALLAXIS) / sizeof(ALLAXIS[0]); i++) {
        if(a->configOption.opt11 > 0){delay(a->configOption.opt11);}
        scanHelper(a->veh, *a->vehPos, ALLAXIS[i], a->scanMap, true, a->scanCounter, a->configOption);
        if(a->configOption.opt11 > 0){delay(a->configOption.opt11);}
        scanHelper(a->veh, *a->vehPos, ALLAXIS[i], a->scanMap, false, a->scanCounter, a->configOption);
        if (ALLAXIS[i].x == 0 || ALLAXIS[i].y == 0) {
            for (int k = 1; k <= 2; k++) {
                if (a->scanMap[a->vehPos->y + ALLAXIS[i].y * k][a->vehPos->x + ALLAXIS[i].x * k] == '#') {
                    fillOutside(a->scanMap, a->vehPos->x + ALLAXIS[i].x * k, a->vehPos->y + ALLAXIS[i].y * k, ALLAXIS[i], a->flush);
                    break;
                }
            }
        }
    }
    tryVisitE(a);
    shouldStop(a);
}

static void turnLogic(AutoState *a, PosType axis){
    PosType left = leftOfPos(axis);
    PosType right = rightOfPos(axis);
    char valueRight = a->scanMap[a->vehPos->y + right.y][a->vehPos->x + right.x], valueRight2 = a->scanMap[a->vehPos->y + right.y * 2][a->vehPos->x + right.x * 2];
    char valueLeft = a->scanMap[a->vehPos->y + left.y][a->vehPos->x + left.x], valueLeft2 = a->scanMap[a->vehPos->y + left.y * 2][a->vehPos->x + left.x * 2];

    if(isWall(valueLeft) || isWall(valueLeft2)) {
        a->facing = turnClockwise(a->facing, 90);
    } else if(isWall(valueRight) || isWall(valueRight2)) {
        a->facing = turnAntiClockwise(a->facing, 90);
    } else {
        int costLeft = a->costLog[(unsigned char)valueLeft].totalCost + a->costLog[(unsigned char)valueLeft2].totalCost;
        int costRight = a->costLog[(unsigned char)valueRight].totalCost + a->costLog[(unsigned char)valueRight2].totalCost;
        (costLeft > costRight) ? (a->facing = turnClockwise(a->facing, 90)) : (a->facing = turnAntiClockwise(a->facing, 90));
    }
}

static void chooseFacingDir(AutoState *a) {
    DirectionType sideA = a->facing, sideB = turnClockwise(sideA, 180);
    int distA = 0, distB = 0;
    bool knownA = false, knownB = false;
    switch (sideA) {
        case DIR_NORTH: distA = a->bound.fTop - (ORIGINY - a->vehPos->y); knownA = (a->bound.top != -1); break;
        case DIR_SOUTH: distA = a->bound.fBottom - (a->vehPos->y - ORIGINY); knownA = (a->bound.bottom != -1); break;
        case DIR_EAST:  distA = a->bound.fRight - (a->vehPos->x - ORIGINX); knownA = (a->bound.right != -1); break;
        case DIR_WEST:  distA = a->bound.fLeft - (ORIGINX - a->vehPos->x); knownA = (a->bound.left != -1); break;
    }
    switch (sideB) {
        case DIR_NORTH: distB = a->bound.fTop - (ORIGINY - a->vehPos->y); knownB = (a->bound.top != -1); break;
        case DIR_SOUTH: distB = a->bound.fBottom - (a->vehPos->y - ORIGINY); knownB = (a->bound.bottom != -1); break;
        case DIR_EAST:  distB = a->bound.fRight - (a->vehPos->x - ORIGINX); knownB = (a->bound.right != -1); break;
        case DIR_WEST:  distB = a->bound.fLeft - (ORIGINX - a->vehPos->x); knownB = (a->bound.left != -1); break;
    }
    if (!knownA && knownB) {
        a->facing = sideA;
    } else if (knownA && !knownB) {
        a->facing = sideB;
    } else {
        a->facing = (distA >= distB) ? sideA : sideB;
    }
}

static int stepToEdge(DirectionType dir, BoundLength mapBound, PosType *vehPos) {
    int remaining;
    switch (dir) {
        //remaining Length = total length of boundary from origin - (distance from vehicle to origin)
        case DIR_NORTH: remaining = mapBound.fTop - (ORIGINY - vehPos->y); break;
        case DIR_SOUTH: remaining = mapBound.fBottom - (vehPos->y - ORIGINY); break;
        case DIR_EAST:  remaining = mapBound.fRight - (vehPos->x - ORIGINX); break;
        case DIR_WEST:  remaining = mapBound.fLeft - (ORIGINX - vehPos->x); break;
        default:
            return 0;
    }
    return (remaining > 1) ? remaining - 1 : 0;
}

static void boundaryLogger(AutoState *a, PosType axis) {
    int k = isWall(a->scanMap[a->vehPos->y + axis.y][a->vehPos->x + axis.x]) ? 1 : isWall(a->scanMap[a->vehPos->y + 2*axis.y][a->vehPos->x + 2*axis.x]) ? 2 : 0;
    switch (a->facing) {
        case DIR_NORTH: a->bound.top = a->vehPos->y + axis.y*k; a->bound.fTop = abs(a->vehPos->y - ORIGINY); break;
        case DIR_SOUTH: a->bound.bottom = a->vehPos->y + axis.y*k; a->bound.fBottom = abs(a->vehPos->y - ORIGINY); break;
        case DIR_EAST:  a->bound.right = a->vehPos->x + axis.x*k; a->bound.fRight = abs(a->vehPos->x - ORIGINX); break;
        case DIR_WEST:  a->bound.left = a->vehPos->x + axis.x*k; a->bound.fLeft = abs(a->vehPos->x - ORIGINX); break;
        default: break;
    }
}

static void runToWall(AutoState *a) {
    PosType axis = dirToAxis(a->facing);
    while (!a->GGNOOBS && !isWall(a->scanMap[a->vehPos->y + axis.y][a->vehPos->x + axis.x]) || !isWall(a->scanMap[a->vehPos->y + 2*axis.y][a->vehPos->x + 2*axis.x])) {
        stepAndScan(a);
    }
    if (a->GGNOOBS) return;
    boundaryLogger(a, axis);
    stepAndScan(a);
    if (a->GGNOOBS) return;
    turnLogic(a, axis);
}

static void runSteps(AutoState *a, int step) {
    PosType axis = dirToAxis(a->facing);
    for (int i = 0; i < step && !a->GGNOOBS; i++) {   
        if (isWall(a->scanMap[a->vehPos->y + axis.y][a->vehPos->x + axis.x]) || isWall(a->scanMap[a->vehPos->y + 2*axis.y][a->vehPos->x + 2*axis.x])) { boundaryLogger(a, axis); break; }
        stepAndScan(a);
    }
    if (a->GGNOOBS) return;
    turnLogic(a, axis);
}

static void sweep(AutoState *a) {
        if (a->flush) {
            runToWall(a);
        } else {
            runSteps(a, stepToEdge(a->facing, a->bound, a->vehPos));
        }
}

void autoRun(VehDataType *vehicleData, PosType *vehPos, terrainCostType terrainCostLog[256], char scanMap[HEIGHT][WIDTH], char exploreMap[HEIGHT][WIDTH], int *scanCounter, int *moveCounter, int missionType, Option *configOption) {
    DirectionType StartFace = DIR_SOUTH;
    AutoState a = {
        .veh = vehicleData, .vehPos = vehPos, .costLog = terrainCostLog, .configOption = configOption,
        .scanMap = scanMap, .exploreMap = exploreMap,
        .scanCounter = scanCounter, .moveCounter = moveCounter,
        .missionType = missionType, .facing = StartFace, .bound = {-1,-1,-1,-1}, 
        .flush = false, .GGNOOBS = false
    };
    if(missionType == 8) a.flush = true;
    initAuto(&a);

    if(missionType == 7) { a.flush = true; runToWall(&a); runToWall(&a); runToWall(&a); runToWall(&a); runToWall(&a); runToWall(&a); } else {
        runToWall(&a);
        a.facing = turnClockwise(StartFace, 180);
        runToWall(&a);
        runToWall(&a);

        while (!a.GGNOOBS && (a.bound.top == -1 || a.bound.bottom == -1 || a.bound.right == -1 || a.bound.left == -1)) {
            chooseFacingDir(&a);
            sweep(&a);
            chooseFacingDir(&a);
            runSteps(&a, 5);
        }
        chooseFacingDir(&a);
        runToWall(&a);
    }
    printf("Bounds -> top: %d  bottom: %d  left: %d  right: %d\n", a.bound.top, a.bound.bottom, a.bound.left, a.bound.right);
    printf("Bounds -> top: %d  bottom: %d  left: %d  right: %d\n", a.bound.fTop, a.bound.fBottom, a.bound.fLeft, a.bound.fRight);
}
