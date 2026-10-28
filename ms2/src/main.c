#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "SatComRelay.h"
#include "mapMake.h"
#include "vehControl.h"
#include "errorHandle.h"
#include "autoControl.h"

void sumTerrain(char map[HEIGHT][WIDTH]) {
    int count[256] = {0};
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if(map[i][j] != '\0') {
                count[(unsigned char)map[i][j]]++;
            }        
        }
    }
    for (int i = 0; i < 256; i++) {
        if (count[i] > 0) {
            printf("Type '%c': %d\n", i, count[i]);
        }
    }
}

VehDataType initVehicle() {
    char scenarioFile[256];
    bool isFileEncrypted;
    bool randomizeStartPosition;
    int missionType;

    printf("\n\nPlease enter the scenario file's name:\t");
    fgets(scenarioFile, sizeof(scenarioFile), stdin);
    scenarioFile[strcspn(scenarioFile, "\n")] = '\0';

    if(getValidChar("\n\nIs the scenario file encrypted?\t(y/n): ", "YN") == 'Y') {isFileEncrypted = true;} else {isFileEncrypted = false;}
    if(getValidChar("\n\nDo you wish to randomize start position?\t(y/n): ", "YN") == 'Y') {randomizeStartPosition = true;} else {randomizeStartPosition = false;}

    printf("\nSelect mission:\n");
    printf("1. Casual Explore\n");
    printf("2. Find 1 End Point\n");
    printf("3. Reach 1 End Point\n");
    printf("4. Find All End Points\n");
    printf("5. Reach ALL End Points\n");
    printf("6. Find ALL Danger Points\n");
    printf("7. Find Map Boundaries\n");
    printf("8. Map Everything\n");
    missionType = getValidChar("\nEnter your choice (1-8): ", "12345678") - '0';

    printf("\n\nfilenname : %s, isFileEncrypted : %s, randomizeStartPosition : %s\n",scenarioFile, isFileEncrypted ? "true" : "false", randomizeStartPosition ? "true" : "false");
    printf("\nMission type: %d",missionType);
    return SCRelay_Signal_Veh_Init(
        scenarioFile,
        isFileEncrypted,
        randomizeStartPosition,
        missionType
    );
}

void showUsageInstructions() {
    printf("\n");
    printf("========================================\n");
    printf("          VEHICLE CONTROLS\n");
    printf("========================================\n");

    printf("\nQ  - Quit\n");
    printf("V  - Show usage instructions\n");

    printf("\nMOVEMENT\n");
    printf("W  - Move North\n");
    printf("S  - Move South\n");
    printf("A  - Move West\n");
    printf("D  - Move East\n");

    printf("\nSCANNING\n");
    printf("T  - Toggle scan radius (1 <-> 2)\n");

    printf("\nINFORMATION\n");
    printf("Z  - Show vehicle status\n");
    printf("C  - Show map of terrain scanned\n");
    printf("X  - Show map of terrain travelled\n");

    printf("Note: In Map display, '_' underline indicates curr. veh. location!");
    printf("\n========================================\n");
    getValidChar("\nPress 'y' key and hit Enter to continue! ", "Y");
}

int main() {
    VehDataType vehicleData = initVehicle();
    char scanMap[HEIGHT][WIDTH];
    char exploreMap[HEIGHT][WIDTH];

    VehPosType vehPos;
    vehPos.x = WIDTH/2;
    vehPos.y = HEIGHT/2;
    
    terrainCostType terrainCostLog[256] = {0};
    char answer = '\0';
    bool radiusBoolOne = true;          //default use case is 1
    int moveCounter = 0;
    int scanCounter = 0;

    mapMake(scanMap);
    mapMake(exploreMap);
    showUsageInstructions();
    while (answer != 'Q') {
        switch (answer = getValidChar("\n\nEnter your command (q to quit) :","WASDVQTIZCXOP12346789")) {
        case 'W':
            moveHelper(&vehicleData, &vehPos ,terrainCostLog ,scanMap, exploreMap, 0, 1, &moveCounter);
            break;
         
        case 'S':			
            moveHelper(&vehicleData, &vehPos ,terrainCostLog ,scanMap, exploreMap, 0, -1, &moveCounter);
            break;

        case 'A':
            moveHelper(&vehicleData, &vehPos ,terrainCostLog ,scanMap, exploreMap, -1, 0, &moveCounter);
            break;

        case 'D':
            moveHelper(&vehicleData, &vehPos ,terrainCostLog ,scanMap, exploreMap, 1, 0, &moveCounter);
            break;

        case '1':  
            scanHelper(&vehicleData, vehPos, scanMap, radiusBoolOne, -1, -1, &scanCounter);
            break;

        case '2':         
            scanHelper(&vehicleData, vehPos, scanMap, radiusBoolOne, 0, -1, &scanCounter);
            break;

        case '3':     
            scanHelper(&vehicleData, vehPos, scanMap, radiusBoolOne, 1, -1, &scanCounter);
            break;

        case '4':         
            scanHelper(&vehicleData, vehPos, scanMap, radiusBoolOne, -1, 0, &scanCounter);
            break;

        case '6':          
            scanHelper(&vehicleData, vehPos, scanMap, radiusBoolOne, 1, 0, &scanCounter);
            break;

        case '7':           
            scanHelper(&vehicleData, vehPos, scanMap, radiusBoolOne, -1, 1, &scanCounter);
            break;

        case '8':          
            scanHelper(&vehicleData, vehPos, scanMap, radiusBoolOne, 0, 1, &scanCounter);
            break;

        case '9':           
            scanHelper(&vehicleData, vehPos, scanMap, radiusBoolOne, 1, 1, &scanCounter);
            break;
            
        case 'T':
            radiusBoolOne = !radiusBoolOne;
            if(radiusBoolOne) {
                printf("Toggle Scan Radius ==> now set to 1 terrain units!");
            } else {
                printf("Toggle Scan Radius ==> now set to 2 terrain units!");
            }
            break;
        case 'I': 
            printf("\nAllocate energy to shield ( 1 to %ld ) :",vehicleData.currEnergy);  
            vehicleData = SCRelay_Signal_Veh_AllocateToShield (getValidLongHelper());
            break;
        case 'Z':
            printf("\nVehicle Status / Statistics :\n");
            printf("\nInitial Energy: %ld, Total Energy Used: %ld, Energy left: %ld, Shield Energy Left: %ld\n",vehicleData.initialEnergy,vehicleData.totEnergyUsed,vehicleData.currEnergy,vehicleData.currShieldEnergy);
            break;
        case 'C':
            printMapScan(scanMap, vehPos, 0, 0, '\0');
            break;
        case 'X':
            printMapStatic(exploreMap,vehPos);
            break;
        case 'Q':
            printf("\n================================\n");
            printf("          EXPLORATION REPORT      \n");
            printf("================================\n\n");
            printf("Total no. of scans : %d", scanCounter);
            printf("\nTotal no. of movements (steps) taken : %d", moveCounter);
            printf("\n\n[ Energy Report ]\n");
            printf("\nInitial Energy\t\t: %ld", vehicleData.initialEnergy);
            printf("\nTotal Energy Used\t: %ld", vehicleData.totEnergyUsed);
            printf("\nTotal Shield Energy used: %ld", vehicleData.totShieldEnergyUsed);
            printf("\nEnergy Left\t\t: %ld", vehicleData.currEnergy);
            printf("\nShield Energy Left\t: %ld", vehicleData.currShieldEnergy);

            printf("\n\n[Terrain Types Scanned ]\n\n");
            sumTerrain(scanMap);
            printf("\n\n[Terrain Types Explored ]\n\n");
            sumTerrain(exploreMap);
            printf("\n");
            for (int i = 0; i < 256; i++) {
                if (terrainCostLog[i].totalCost > 0) {
                    printf("Type '%c': %d : %d : %d\n", i, terrainCostLog[i].energy,terrainCostLog[i].shield,terrainCostLog[i].totalCost);
                }
            }
            SCRelay_Signal_Veh_Close();
            break;              
        case 'V':
            showUsageInstructions();
            break;
        case 'O':
            autoConfig();
            break;
        case 'P':
            autoRun(&vehicleData, &vehPos, terrainCostLog, scanMap, exploreMap, &scanCounter, &moveCounter);
            break;
        }
    }
}