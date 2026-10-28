#ifndef AUTOCONTROL_H
#define AUTOCONTROL_H

#include "config.h"
#include <stdbool.h>
#include "SatComRelay.h"
#include "mapMake.h"
#include "vehControl.h"

void autoConfig();

void autoRun(
    VehDataType *vehicleData,
    VehPosType *vehPos,
    terrainCostType terrainCostLog[256],
    char scanMap[HEIGHT][WIDTH],
    char exploreMap[HEIGHT][WIDTH],
    int *scanCounter,
    int *moveCounter
);
#endif
