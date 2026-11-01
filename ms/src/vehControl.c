#include <stdio.h>
#include "vehControl.h"
#include "SatComRelay.h"
#include "mapMake.h"

static char performScan(VehDataType *vehicleData, PosType axis, bool radiusBoolOne) {
    if (radiusBoolOne) {
        if (axis.x == NORTH.x && axis.y == NORTH.y)         return SCRelay_Signal_Veh_ScanNorth(vehicleData);
        if (axis.x == NORTHEAST.x && axis.y == NORTHEAST.y) return SCRelay_Signal_Veh_ScanNorthEast(vehicleData);
        if (axis.x == EAST.x && axis.y == EAST.y)           return SCRelay_Signal_Veh_ScanEast(vehicleData);
        if (axis.x == SOUTHEAST.x && axis.y == SOUTHEAST.y) return SCRelay_Signal_Veh_ScanSouthEast(vehicleData);
        if (axis.x == SOUTH.x && axis.y == SOUTH.y)         return SCRelay_Signal_Veh_ScanSouth(vehicleData);
        if (axis.x == SOUTHWEST.x && axis.y == SOUTHWEST.y) return SCRelay_Signal_Veh_ScanSouthWest(vehicleData);
        if (axis.x == WEST.x && axis.y == WEST.y)           return SCRelay_Signal_Veh_ScanWest(vehicleData);
        if (axis.x == NORTHWEST.x && axis.y == NORTHWEST.y) return SCRelay_Signal_Veh_ScanNorthWest(vehicleData);
    } else {
        if (axis.x == NORTH.x && axis.y == NORTH.y)         return SCRelay_Signal_Veh_ScanNorth2(vehicleData);
        if (axis.x == NORTHEAST.x && axis.y == NORTHEAST.y) return SCRelay_Signal_Veh_ScanNorthEast2(vehicleData);
        if (axis.x == EAST.x && axis.y == EAST.y)           return SCRelay_Signal_Veh_ScanEast2(vehicleData);
        if (axis.x == SOUTHEAST.x && axis.y == SOUTHEAST.y) return SCRelay_Signal_Veh_ScanSouthEast2(vehicleData);
        if (axis.x == SOUTH.x && axis.y == SOUTH.y)         return SCRelay_Signal_Veh_ScanSouth2(vehicleData);
        if (axis.x == SOUTHWEST.x && axis.y == SOUTHWEST.y) return SCRelay_Signal_Veh_ScanSouthWest2(vehicleData);
        if (axis.x == WEST.x && axis.y == WEST.y)           return SCRelay_Signal_Veh_ScanWest2(vehicleData);
        if (axis.x == NORTHWEST.x && axis.y == NORTHWEST.y) return SCRelay_Signal_Veh_ScanNorthWest2(vehicleData);
    }
    return '\0';
}

static void performMove(VehDataType *vehicleData, PosType axis) {
    if (axis.x == NORTH.x && axis.y == NORTH.y)     *vehicleData = SCRelay_Signal_Veh_MoveUpNorth();
    if (axis.x == EAST.x && axis.y == EAST.y)    *vehicleData = SCRelay_Signal_Veh_MoveRightEast();
    if (axis.x == SOUTH.x && axis.y == SOUTH.y)   *vehicleData = SCRelay_Signal_Veh_MoveDownSouth();
    if (axis.x == WEST.x && axis.y == WEST.y)    *vehicleData = SCRelay_Signal_Veh_MoveLeftWest();
}

static PosType multiplyPos(PosType pos, int multiplier) {
    return (PosType){
        .x = pos.x * multiplier,
        .y = pos.y * multiplier
    };
}

void scanHelper(VehDataType *vehicleData, PosType vehPos, PosType localPos, char scanMap[HEIGHT][WIDTH], bool radiusBoolOne, int *Counter) {
    printf("\nInitial Energy: %ld, Total Energy Used: %ld, Energy left: %ld, Shield Energy Left: %ld\n",vehicleData->initialEnergy,vehicleData->totEnergyUsed,vehicleData->currEnergy,vehicleData->currShieldEnergy);
    char value = performScan(vehicleData, localPos, radiusBoolOne);
    int step = radiusBoolOne ? 1 : 2;
    printf("Returned (scanned) terrain symbol is : %c", value);
    (*Counter)++;
    localPos = multiplyPos(localPos,step);
    printMapScan(scanMap, vehPos, localPos, value);    
}

void moveHelper(VehDataType *vehicleData, PosType *vehPos, PosType localPos, terrainCostType terrainCostLog[256], char scanMap[HEIGHT][WIDTH], char exploreMap[HEIGHT][WIDTH], int *Counter) {
    if(scanMap[vehPos->y + localPos.y][vehPos->x + localPos.x] == '\0') {
        char value = performScan(vehicleData, localPos, true);
        if(value != '#') {
            int startEnergyCost = vehicleData->currEnergy;
            int startShieldCost = vehicleData->currShieldEnergy;
            printf("\nInitial Energy: %ld, Total Energy Used: %ld, Energy left: %ld, Shield Energy Left: %ld\n",vehicleData->initialEnergy,vehicleData->totEnergyUsed,vehicleData->currEnergy,vehicleData->currShieldEnergy);
            performMove(vehicleData, localPos);
            vehPos->y = vehPos->y + localPos.y;
            vehPos->x = vehPos->x + localPos.x;
            terrainCostLog[value].energy = startEnergyCost - vehicleData->currEnergy;
            terrainCostLog[value].shield = startShieldCost - vehicleData->currShieldEnergy;
            terrainCostLog[value].totalCost = terrainCostLog[value].energy + terrainCostLog[value].shield;
            printMapExplore(exploreMap, *vehPos, value);
            (*Counter)++;
        } else {
            printf("Cannot move! Border detected.\n");
        }                
    } else if (scanMap[vehPos->y + localPos.y][vehPos->x + localPos.x] != '#') {
            int startEnergyCost = vehicleData->currEnergy;
            int startShieldCost = vehicleData->currShieldEnergy;
            printf("\nInitial Energy: %ld, Total Energy Used: %ld, Energy left: %ld, Shield Energy Left: %ld\n",vehicleData->initialEnergy,vehicleData->totEnergyUsed,vehicleData->currEnergy,vehicleData->currShieldEnergy);
            performMove(vehicleData, localPos);
            vehPos->y = vehPos->y + localPos.y;
            vehPos->x = vehPos->x + localPos.x;
            terrainCostLog[scanMap[vehPos->y][vehPos->x]].energy = startEnergyCost - vehicleData->currEnergy;
            terrainCostLog[scanMap[vehPos->y][vehPos->x]].shield = startShieldCost - vehicleData->currShieldEnergy;
            terrainCostLog[scanMap[vehPos->y][vehPos->x]].totalCost = terrainCostLog[scanMap[vehPos->y][vehPos->x]].energy + terrainCostLog[scanMap[vehPos->y][vehPos->x]].shield;
            printMapExplore(exploreMap, *vehPos, scanMap[vehPos->y][vehPos->x]);
            (*Counter)++;
    } else {
        printf("Cannot move! Border detected.\n");
    }
}
