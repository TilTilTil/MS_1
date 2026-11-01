#ifndef VEHCONTROL_H
#define VEHCONTROL_H

#include "config.h"
#include <stdbool.h>
#include "SatComRelay.h"
#include "mapMake.h"

typedef struct{
    int energy;
    int shield;
    int totalCost;
}terrainCostType;

void scanHelper(VehDataType *vehicleData, 
                PosType vehPos, 
                PosType localPos,
                char scanMap[HEIGHT][WIDTH], 
                bool radiusBoolOne, 
                int *scanCounter);

void moveHelper(VehDataType *vehicleData, 
                PosType *vehPos,
                PosType localPos,
                terrainCostType terrainCostLog[256],
                char scanMap[HEIGHT][WIDTH], 
                char exploreMap[HEIGHT][WIDTH], 
                int *moveCounter);

#endif
