#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "SatComRelay.h"
#include "mapMake.h"
#include "vehControl.h"
#include "helpers.h"
#include "autoControl.h"

void sumTerrain(char map[HEIGHT][WIDTH]) {
    int count[256] = {0};
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if(map[i][j] != '\0' && map[i][j] != FILL) {
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

char *trimFile(char *s) {
    if (!s) return s;
    while (isspace((unsigned char)*s)) s++;
    if (*s == '\0') return s;
    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';

    return s;
}

VehDataType initVehicle(int *missionType) {
char scenarioFile[256] = {0};
    bool isFileEncrypted = false;
    bool randomizeStartPosition = false;

    char *input = readline("\n\nPlease enter the scenario file's name:\t");
    if (input) {
        char *trimmed = trimFile(input);
        snprintf(scenarioFile, sizeof(scenarioFile), "%s", trimmed);
        free(input);
    }

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
    *missionType = getValidChar("\nEnter your choice (1-8): ", "12345678") - '0';

    printf("\n\nfilenname : %s, isFileEncrypted : %s, randomizeStartPosition : %s\n",scenarioFile, isFileEncrypted ? "true" : "false", randomizeStartPosition ? "true" : "false");
    printf("\nMission type: %d",*missionType);
    return SCRelay_Signal_Veh_Init(
        scenarioFile,
        isFileEncrypted,
        randomizeStartPosition,
        *missionType
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
    printf("O  - Configure controls\n");

    printf("\nAUTO\n");
    printf("P  - Auto mode start!\n");
    
    printf("Note: In Map display, '_' underline indicates curr. veh. location!");
    printf("\n========================================\n");
    getValidChar("\nPress 'y' key and hit Enter to continue! ", "Y");
}

    void autoConfig(Option *configOption) {     //lowkey should put this in main
        printf("--- Vehicle Configuration Menu ---\n\n");

        // Option chuck 1
        if(getValidChar("\n\n1. After every action, show vehicle status / energy stats (y/n): \n", "YN") == 'Y') {
            configOption->opt2 = true;
            configOption->opt3 = true;
            configOption->opt4 = true;
        } else {
            if(getValidChar("\n2. After every scan action, show vehicle status / energy stats (y/n): \n", "YN") == 'N') {
                configOption->opt2 = false;
            } else { configOption->opt2 = true;}
            if(getValidChar("\n3. After every move action, show vehicle status / energy stats (y/n): \n", "YN") == 'N') {
                configOption->opt3 = false;
            } else { configOption->opt3 = true;}
            if(getValidChar("\n4. After every shield allocation, show vehicle status / energy stats (y/n): \n", "YN") == 'N') {
                configOption->opt4 = false;
            } else { configOption->opt4 = true;}
        }
        // option chuck 2
        if(getValidChar("\n5. After every scan action, display 'current map' of terrain scanned by vehicle so far (y/n): \n", "YN") == 'N') {
            configOption->opt5 = false;
        } else { configOption->opt5 = true;}
        if(getValidChar("\n6. After every move action, display 'current map' of terrain travelled by vehicle so far (y/n): \n", "YN") == 'N') {
            configOption->opt6 = false;
        } else { configOption->opt6 = true;}
        printf("\n7. Enter 't1' secs delay, after displaying vehicle status / energy stats: \n");
        configOption->opt7 = getValidIntHelper();
        printf("\n8. Enter 't2' secs delay, after displaying 'current map' of terrain scanned: \n");
        configOption->opt8 = getValidIntHelper();
        printf("\n9. Enter 't3' secs delay, after displaying 'current map' of terrain travelled: \n");
        configOption->opt9 = getValidIntHelper();

        //option chuck 3
        printf("10. Enter 't4' secs delay, after any action taken by auto-pilot function: \n");
        configOption->opt10 = getValidIntHelper();
        if(configOption->opt10 > 0) {
            configOption->opt11 = configOption->opt12 = configOption->opt13 = configOption->opt10;
        } else {
            printf("\n11. Enter 't5' secs delay, after each scan action taken by auto-pilot function: \n");
            configOption->opt11 = getValidIntHelper();
            printf("\n12. Enter 't6' secs delay, after each move taken by auto-pilot function: \n");
            configOption->opt12 = getValidIntHelper();
            printf("\n13. Enter 't7' secs delay, after each shield allocation action taken by auto-pilot function: \n");
            configOption->opt13 = getValidIntHelper();
        }

        //option chunk 4
        printf("\n14. Enter 'n1' Pause auto pilot when current energy avaliable falls below: \n");
        configOption->opt14 = getValidLongHelper();
        printf("\n15. Enter 'n2' Pause auto pilot when current shield avaliable falls below: \n");
        configOption->opt15 = getValidLongHelper();     
        printf("\n16. Enter 'n3' Pause auto pilot when total number of move exceeds: \n");
        configOption->opt16 = getValidIntHelper();
        printf("\n17. Enter 'n4' Pause auto pilot when total number of scans exceeds: \n");
        configOption->opt17 = getValidIntHelper();

        printf("\n--- Configuration Summary ---\n");
        printf("Option 1 (All Status): %d\n", configOption->opt1);
        printf("Option 2 (Scan Status): %d\n", configOption->opt2);
        printf("Option 3 (Move Status): %d\n", configOption->opt3);
        printf("Option 4 (Shield Status): %d\n", configOption->opt4);
        printf("Option 5 (Scan Map): %d\n", configOption->opt5);
        printf("Option 6 (Travel Map): %d\n", configOption->opt6);
        printf("Option 7 (Status Display Delay): %ld seconds\n", configOption->opt7);
        printf("Option 8 (Scanned Map Display Delay): %ld seconds\n", configOption->opt8);
        printf("Option 9 (Travelled Map Display Delay): %ld seconds\n", configOption->opt9);
        printf("Option 10 (Auto-Pilot Action Delay): %ld seconds\n", configOption->opt10);
        printf("Option 11 (Auto-Pilot Scan Delay): %ld seconds\n", configOption->opt11);
        printf("Option 12 (Auto-Pilot Move Delay): %ld seconds\n", configOption->opt12);
        printf("Option 13 (Auto-Pilot Shield Allocation Delay): %ld seconds\n", configOption->opt13);
    }

int main() {
    int missionType = 0;
    VehDataType vehicleData = initVehicle(&missionType);
    char scanMap[HEIGHT][WIDTH];
    char exploreMap[HEIGHT][WIDTH];

    PosType vehPos;
    vehPos.x = ORIGINX;
    vehPos.y = ORIGINY;
    
    terrainCostType terrainCostLog[256] = {0};
    Option configOption = {
        .opt1  = SHOW_STATUS_AFTER_ANY_ACTION,
        .opt2  = SHOW_STATUS_AFTER_SCAN,
        .opt3  = SHOW_STATUS_AFTER_MOVE,
        .opt4  = SHOW_STATUS_AFTER_SHIELD,
        .opt5  = SHOW_MAP_AFTER_SCAN,
        .opt6  = SHOW_MAP_AFTER_MOVE,
        .opt7  = DELAY_AFTER_STATUS_DISPLAY,
        .opt8  = DELAY_AFTER_SCAN_MAP,
        .opt9  = DELAY_AFTER_TRAVEL_MAP,
        .opt10 = DELAY_AFTER_ANY_AUTOPILOT_ACTION,
        .opt11 = DELAY_AFTER_AUTOPILOT_SCAN,
        .opt12 = DELAY_AFTER_AUTOPILOT_MOVE,
        .opt13 = DELAY_AFTER_AUTOPILOT_SHIELD,
        .opt14 = ENERGY_LIMIT,
        .opt15 = SHIELD_LIMIT,
        .opt16 = MOVE_LIMIT,
        .opt17 = SCAN_LIMIT,
        .autoOn = false
    };    
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
            moveHelper(&vehicleData, &vehPos , NORTH ,terrainCostLog ,scanMap, exploreMap, &moveCounter, configOption);
            break;

        case 'S':			
            moveHelper(&vehicleData, &vehPos , SOUTH ,terrainCostLog ,scanMap, exploreMap, &moveCounter, configOption);
            break;

        case 'A':
            moveHelper(&vehicleData, &vehPos , WEST ,terrainCostLog ,scanMap, exploreMap, &moveCounter, configOption);
            break;

        case 'D':
            moveHelper(&vehicleData, &vehPos , EAST ,terrainCostLog ,scanMap, exploreMap, &moveCounter, configOption);
            break;

        case '1':  
            scanHelper(&vehicleData, vehPos, SOUTHWEST, scanMap, radiusBoolOne,&scanCounter, configOption);
            break;

        case '2':         
            scanHelper(&vehicleData, vehPos, SOUTH, scanMap, radiusBoolOne, &scanCounter, configOption);
            break;

        case '3':     
            scanHelper(&vehicleData, vehPos, SOUTHEAST, scanMap, radiusBoolOne, &scanCounter, configOption);
            break;

        case '4':         
            scanHelper(&vehicleData, vehPos, WEST, scanMap, radiusBoolOne, &scanCounter, configOption);
            break;

        case '6':          
            scanHelper(&vehicleData, vehPos, EAST, scanMap, radiusBoolOne, &scanCounter, configOption);
            break;

        case '7':           
            scanHelper(&vehicleData, vehPos, NORTHWEST, scanMap, radiusBoolOne, &scanCounter, configOption);
            break;

        case '8':          
            scanHelper(&vehicleData, vehPos, NORTH, scanMap, radiusBoolOne, &scanCounter, configOption);
            break;

        case '9':           
            scanHelper(&vehicleData, vehPos, NORTHEAST, scanMap, radiusBoolOne, &scanCounter, configOption);
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
            if(configOption.opt4){statusHelper(&vehicleData, configOption);};
            break;
        case 'Z':
            printf("\nVehicle Status / Statistics :\n");
            statusHelper(&vehicleData, configOption);
            break;
        case 'C':
            PosType origin = {.x = 0,.y = 0};
            printMapScan(scanMap, vehPos, origin, '\0');
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
            autoConfig(&configOption);
            break;
        case 'P':
            autoRun(&vehicleData, &vehPos, terrainCostLog, scanMap, exploreMap, &scanCounter, &moveCounter, missionType, &configOption);
            break;
        }
    }
}