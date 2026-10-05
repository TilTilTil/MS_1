#ifndef VEHCONTROL_H
#define VEHCONTROL_H

#include "config.h"
#include <stdbool.h>
#include "SatComRelay.h"
#include "mapMake.h"

void scanHelper(VehDataType *vehicleData, 
                VehPosType vehPos, 
                char scanMap[HEIGHT][WIDTH], 
                bool radiusBoolOne, 
                int localX, 
                int localY,
                int *scanCounter);

void moveHelper(VehDataType *vehicleData, 
                VehPosType *vehPos, 
                char scanMap[HEIGHT][WIDTH], 
                char exploreMap[HEIGHT][WIDTH], 
                int localX, 
                int localY,
                int *moveCounter);

#endif
