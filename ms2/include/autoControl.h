#ifndef AUTOCONTROL_H
#define AUTOCONTROL_H

#include "config.h"
#include <stdbool.h>
#include "SatComRelay.h"
#include "mapMake.h"
#include "vehControl.h"
#include "helpers.h"

typedef enum {
    DIR_NORTH,
    DIR_EAST,
    DIR_SOUTH,
    DIR_WEST
} DirectionType;

typedef struct {
    int top;
    int right;
    int bottom;
    int left;

    //functional Boundary
    int fTop;
    int fRight;
    int fBottom;
    int fLeft;
} BoundLength;

typedef struct {
    VehDataType *veh;
    PosType *vehPos;
    DirectionType facing;
    BoundLength bound;
    terrainCostType *costLog;
    Option configOption;
    char (*scanMap)[WIDTH];
    char (*exploreMap)[WIDTH];
    char target;
    int *scanCounter;
    int *moveCounter;
    int missionType;
    bool  flush;
    bool GGNOOBS;
    bool eVisited[HEIGHT][WIDTH];
    int  eVisitedCount;
} AutoState;

void autoRun(
    VehDataType *vehicleData,
    PosType *vehPos,
    terrainCostType terrainCostLog[256],
    char scanMap[HEIGHT][WIDTH],
    char exploreMap[HEIGHT][WIDTH],
    int *scanCounter,
    int *moveCounter,
    int missionType,
    Option *configOption);

#endif
