#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

#include "vehControl.h"
#include "SatComRelay.h"
#include "mapMake.h"
#include "errorHandle.h"
#include "autoControl.h"

#define STARTSHIELD 10000
#define FILL '\1'
#define NUM_ENDS 3
#define ENDPOINT 'E'
#define NUM_DANGER 1
#define DANGERPOINT 'X'
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

static bool wallAhead(AutoState *s, PosType axis) {
    return isWall(s->scanMap[s->vehPos->y + axis.y][s->vehPos->x + axis.x]) || isWall(s->scanMap[s->vehPos->y + 2*axis.y][s->vehPos->x + 2*axis.x]);
}

static bool boundsUnKnown(BoundLength Len) {
    return (Len.top == -1 || Len.bottom == -1 || Len.right == -1 || Len.left == -1);
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
static DirectionType axisToDir(PosType axis) {
    if (axis.y < 0) return DIR_NORTH;
    if (axis.y > 0) return DIR_SOUTH;
    if (axis.x > 0) return DIR_EAST;
    if (axis.x < 0) return DIR_WEST;
}

void autoConfig() {     //lowkey should put this in main
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


static bool eVisitedMap[HEIGHT][WIDTH];
static int  eVisitedCount = 0;
 
static void stepDir(AutoState *a, PosType axis) {
    char next = a->scanMap[a->vehPos->y + axis.y][a->vehPos->x + axis.x];
    *a->veh = SCRelay_Signal_Veh_AllocateToShield(a->costLog[next].shield);
    moveHelper(a->veh, a->vehPos, axis, a->costLog, a->scanMap, a->exploreMap, a->moveCounter);
}

static int sign(int v) { return (v > 0) - (v < 0); }

static bool goToE(AutoState *a) {
    for (size_t i = 0; i < sizeof(ALLAXIS) / sizeof(ALLAXIS[0]); i++) {
        PosType path;
         PosType steps[4];   //highest steps in a path is 4
        int num = 0;
        if (a->scanMap[a->vehPos->y + ALLAXIS[i].y][a->vehPos->x + ALLAXIS[i].x] == ENDPOINT && !eVisitedMap[a->vehPos->y + ALLAXIS[i].y][a->vehPos->x + ALLAXIS[i].x]) {
            path = ALLAXIS[i];
        } else if (a->scanMap[a->vehPos->y + ALLAXIS[i].y * 2][a->vehPos->x + ALLAXIS[i].x * 2] == ENDPOINT && !eVisitedMap[a->vehPos->y + ALLAXIS[i].y * 2][a->vehPos->x + ALLAXIS[i].x * 2]) {
            path = (PosType){ ALLAXIS[i].x * 2, ALLAXIS[i].y * 2 };
        } else {
            continue;
        }

        for (int i = 0; i < abs(path.y); i++) steps[num++] = (PosType){ 0, sign(path.y) };
        for (int i = 0; i < abs(path.x); i++) steps[num++] = (PosType){ sign(path.x), 0 };

        for (int i = 0; i < num; i++) {
            stepDir(a, steps[i]);
        }
        for (int i = num - 1; i >= 0; i--) {
            stepDir(a, (PosType){ -steps[i].x, -steps[i].y });
        }
        eVisitedMap[a->vehPos->y + path.y][a->vehPos->x + path.x] = true;
        eVisitedCount++;
        return true;
    }
    return false;
}

static void tryVisitE(AutoState *a) {
    if (a->GGNOOBS) return;
    if (a->missionType == 3 || a->missionType == 5) {
        goToE(a);
    }
}

static int countTarget(char map[HEIGHT][WIDTH], char value) {   //super shit way to do it but it works lol
    int count = 0;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (map[y][x] == value) {
                count++;
            }
        }
    }
    return count;
}

static bool neverStop( AutoState *a) { 
    (void)a; return false; 
}

static bool countoneE( AutoState *a) {
    return countTarget(a->scanMap, ENDPOINT) >= 1;
}

static bool countE( AutoState *a) {
    return countTarget(a->scanMap, ENDPOINT) >= NUM_ENDS;
}

static bool visitedOneE( AutoState *a) {
    (void)a;
    return eVisitedCount >= 1;
}
 
static bool visitedAllE( AutoState *a) {
    (void)a;
    return eVisitedCount >= NUM_ENDS;
}

static bool countDanger( AutoState *a) {
    return countTarget(a->scanMap, DANGERPOINT) >= NUM_DANGER;
}

static bool checkShield( AutoState *a) {
    return a->veh->currShieldEnergy < 777;
}

static void recoverShield( AutoState *a) {
    printf("\nAllocate some shield then!\n");
    printf("Your shield by the way --> %ld\n", a->veh->totEnergyUsed);
    *a->veh = SCRelay_Signal_Veh_AllocateToShield (getValidLongHelper());
}

static const Condition missionStatement[] = {
    [0] = { 1, neverStop, NULL },  // 1. Casual Explore bruh bruh
    [1] = { 1, countoneE,    NULL },  // 2. Find 1 End Point DONE
    [2] = { 1, visitedOneE,    NULL },  // 3. Reach 1 End Point TODO DONE
    [3] = { 1, countE,    NULL },  // 4. Find All End Points DONE
    [4] = { 1, visitedAllE, NULL },  // 5. Reach ALL End Points TODO DONE
    [5] = { 1, countDanger, NULL },  // 6. Find ALL Danger Points DONE
    [6] = { 1, neverStop, NULL },  // 7. Find Map Boundaries TODO DONE
    [7] = { 1, neverStop, NULL },  // 8. Map Everything DONE
};

static const Condition pauseConditions[] = {
    { 1, checkShield, recoverShield},
};

static bool shouldStop(AutoState *a) {
    if (a->GGNOOBS) return true;
    int m = a->missionType - 1;
    size_t n = sizeof(missionStatement) / sizeof(missionStatement[0]);
    if (m >= 0 && (size_t)m < n && missionStatement[m].check(a)) {
        a->GGNOOBS = true;
        return true;
    }
    for (size_t i = 0; i < sizeof(pauseConditions) / sizeof(pauseConditions[0]); i++) {
        if (pauseConditions[i].check(a)) {
            if (getValidChar("Continue auto-pilot? (y/n): \n", "YN") == 'N') {
                a->GGNOOBS = true;
                return true;
            } else {
                pauseConditions[i].helpMe(a);
            }
        }
    }
    return false;
}

static void initAuto(AutoState *a) {
    *a->veh = SCRelay_Signal_Veh_AllocateToShield(STARTSHIELD);
    for (int i = 0; i < sizeof(ALLAXIS) / sizeof(ALLAXIS[0]); i++) {
        scanHelper(a->veh, *a->vehPos, ALLAXIS[i], a->scanMap, true, a->scanCounter);
        if (shouldStop(a)) return;
        scanHelper(a->veh, *a->vehPos, ALLAXIS[i], a->scanMap, false, a->scanCounter);
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

    *a->veh = SCRelay_Signal_Veh_AllocateToShield(a->costLog[inFront].shield);
    moveHelper(a->veh, a->vehPos, axis, a->costLog, a->scanMap, a->exploreMap, a->moveCounter);

    for (int i = 0; i < sizeof(ALLAXIS) / sizeof(ALLAXIS[0]); i++) {
        scanHelper(a->veh, *a->vehPos, ALLAXIS[i], a->scanMap, true, a->scanCounter);
        scanHelper(a->veh, *a->vehPos, ALLAXIS[i], a->scanMap, false, a->scanCounter);
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
    while (!a->GGNOOBS && !wallAhead(a, axis)) {   // added GGNOOBS check
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
    for (int i = 0; i < step && !a->GGNOOBS; i++) {   // added GGNOOBS check
        if (wallAhead(a, axis)) { boundaryLogger(a, axis); break; }
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

void autoRun(VehDataType *vehicleData, PosType *vehPos, terrainCostType terrainCostLog[256], char scanMap[HEIGHT][WIDTH], char exploreMap[HEIGHT][WIDTH], int *scanCounter, int *moveCounter, int missionType) {
    DirectionType StartFace = DIR_SOUTH;
    AutoState a = {
        .veh = vehicleData, .vehPos = vehPos, .costLog = terrainCostLog,
        .scanMap = scanMap, .exploreMap = exploreMap,
        .scanCounter = scanCounter, .moveCounter = moveCounter,
        .missionType = missionType, .facing = StartFace, .bound = {-1,-1,-1,-1}, 
        .flush = false, .GGNOOBS = false
    };
    if(missionType == 8) a.flush = true;
    initAuto(&a);

    if(missionType == 7) {
        a.flush = true;
        runToWall(&a);
        runToWall(&a);
        runToWall(&a);
        runToWall(&a);
        runToWall(&a);
        runToWall(&a);
    } else {
        runToWall(&a);
        a.facing = turnClockwise(StartFace, 180);
        runToWall(&a);
        runToWall(&a);

        while (!a.GGNOOBS && boundsUnKnown(a.bound)) {
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
