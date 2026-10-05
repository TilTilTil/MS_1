#include <stdio.h>
#include "vehControl.h"
#include "SatComRelay.h"
#include "mapMake.h"

static char performScan(VehDataType *vehicleData, bool radiusBoolOne, int localX, int localY) {
    if (radiusBoolOne) {
        if (localX == 0  && localY == 1)  return SCRelay_Signal_Veh_ScanNorth(vehicleData);
        if (localX == 1  && localY == 1)  return SCRelay_Signal_Veh_ScanNorthEast(vehicleData);
        if (localX == 1  && localY == 0)  return SCRelay_Signal_Veh_ScanEast(vehicleData);
        if (localX == 1  && localY == -1) return SCRelay_Signal_Veh_ScanSouthEast(vehicleData);
        if (localX == 0  && localY == -1) return SCRelay_Signal_Veh_ScanSouth(vehicleData);
        if (localX == -1 && localY == -1) return SCRelay_Signal_Veh_ScanSouthWest(vehicleData);
        if (localX == -1 && localY == 0)  return SCRelay_Signal_Veh_ScanWest(vehicleData);
        if (localX == -1 && localY == 1)  return SCRelay_Signal_Veh_ScanNorthWest(vehicleData);
    } else {
        if (localX == 0  && localY == 1)  return SCRelay_Signal_Veh_ScanNorth2(vehicleData);
        if (localX == 1  && localY == 1)  return SCRelay_Signal_Veh_ScanNorthEast2(vehicleData);
        if (localX == 1  && localY == 0)  return SCRelay_Signal_Veh_ScanEast2(vehicleData);
        if (localX == 1  && localY == -1) return SCRelay_Signal_Veh_ScanSouthEast2(vehicleData);
        if (localX == 0  && localY == -1) return SCRelay_Signal_Veh_ScanSouth2(vehicleData);
        if (localX == -1 && localY == -1) return SCRelay_Signal_Veh_ScanSouthWest2(vehicleData);
        if (localX == -1 && localY == 0)  return SCRelay_Signal_Veh_ScanWest2(vehicleData);
        if (localX == -1 && localY == 1)  return SCRelay_Signal_Veh_ScanNorthWest2(vehicleData);
    }
    return '\0';
}

static void performMove(VehDataType *vehicleData, int localX, int localY) {
    if (localX == 0  && localY == 1)    *vehicleData = SCRelay_Signal_Veh_MoveUpNorth();
    if (localX == 1  && localY == 0)    *vehicleData = SCRelay_Signal_Veh_MoveRightEast();
    if (localX == 0  && localY == -1)   *vehicleData = SCRelay_Signal_Veh_MoveDownSouth();
    if (localX == -1 && localY == 0)    *vehicleData = SCRelay_Signal_Veh_MoveLeftWest();
}

void scanHelper(VehDataType *vehicleData, VehPosType vehPos, char scanMap[HEIGHT][WIDTH], bool radiusBoolOne, int localX, int localY, int *Counter) {
    char value = performScan(vehicleData, radiusBoolOne, localX, localY);
    int step = radiusBoolOne ? 1 : 2;
    printf("Returned (scanned) terrain symbol is : %c", value);
    (*Counter)++;
    printMapScan(scanMap, vehPos, localX * step, localY * step, value);    
}

void moveHelper(VehDataType *vehicleData, VehPosType *vehPos, char scanMap[HEIGHT][WIDTH], char exploreMap[HEIGHT][WIDTH], int localX, int localY, int *Counter) {
    if(scanMap[vehPos->y-localY][vehPos->x + localX] == '\0') {
        char value = performScan(vehicleData, true, localX, localY);
        if(value != '#') {
            performMove(vehicleData, localX, localY);
            vehPos->y = vehPos->y - localY;
            vehPos->x = vehPos->x + localX;
            printMapExplore(exploreMap, *vehPos, value);
            printf("\nInitial Energy: %ld, Total Energy Used: %ld, Energy left: %ld, Shield Energy Left: %ld\n",vehicleData->initialEnergy,vehicleData->totEnergyUsed,vehicleData->currEnergy,vehicleData->currShieldEnergy);
            (*Counter)++;
        } else {
            printf("Cannot move North! Border detected.\n");
        }                
    } else if (scanMap[vehPos->y - localY][vehPos->x + localX] != '#') {
            performMove(vehicleData, localX, localY);
            vehPos->y = vehPos->y - localY;
            vehPos->x = vehPos->x + localX;
            printMapExplore(exploreMap, *vehPos, scanMap[vehPos->y - localY][vehPos->x + localX]);
            printf("\nInitial Energy: %ld, Total Energy Used: %ld, Energy left: %ld, Shield Energy Left: %ld\n",vehicleData->initialEnergy,vehicleData->totEnergyUsed,vehicleData->currEnergy,vehicleData->currShieldEnergy);
            (*Counter)++;
    } else {
        printf("Cannot move North! Border detected.\n");
    }
}
