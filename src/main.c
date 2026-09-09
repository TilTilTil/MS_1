#include <stdio.h>
#include <stdbool.h>
#include "SatComRelay.h"

VehDataType initVehicle() {
    char scenarioFile[256];
    char answer;
    bool isFileEncrypted;
    bool randomizeStartPosition;
    int missionType;

    printf("\n\nPlease enter the scenario file's name:\t");
    scanf("%255s", scenarioFile);

    printf("\n\nIs the scenario file encrypted?\t(y/n): ");
    scanf(" %c", &answer);

    if (answer == 'y' || answer == 'Y')
        isFileEncrypted = true;
    else
        isFileEncrypted = false;

    printf("\n\nDo you wish to randomize start position?\t(y/n): ");
    scanf(" %c", &answer);

    if (answer == 'y' || answer == 'Y')
        randomizeStartPosition = true;
    else
        randomizeStartPosition = false;

    printf("\nSelect mission:\n");
    printf("1. Casual Explore\n");
    printf("2. Find 1 End Point\n");
    printf("3. Reach 1 End Point\n");
    printf("4. Find All End Points\n");
    printf("5. Reach ALL End Points\n");
    printf("6. Find ALL Danger Points\n");
    printf("7. Find Map Boundaries\n");
    printf("8. Map Everything\n");

    printf("\nEnter your choice (1-8): ");
    scanf("%d", &missionType);

    printf("\n\nfilenname : %s, isFileEncrypted : %d, randomizeStartPosition : %d\n", scenarioFile, isFileEncrypted, randomizeStartPosition);

    return SCRelay_Signal_Veh_Init(
        scenarioFile,
        isFileEncrypted,
        randomizeStartPosition,
        missionType
    );
}

int showUsageInstructions(void)
{
    char answer ='\0'; //cannot start with unknown 
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

    while (answer != 'y' && answer != 'Y') {    //Neither y or Y
        printf("\nPress 'y' key and hit Enter to continue! ");
        scanf(" %c", &answer);
    }
    return 0;
}

int main(void) {
    VehDataType vehicleData = initVehicle();
    showUsageInstructions();
    char answer;
    bool radiusBoolOne = true;          //default use case is 1
    while (answer != 'q' && answer != 'Q')
    {
        printf("\nEnter your command (q to quit) :");
        scanf(" %c", &answer);
        switch (answer) {
        case 'w':           
        case 'W':           //Movement
            VehDataType SCRelay_Signal_Veh_MoveUpNorth();
            break;
        case 's':
        case 'S':
            VehDataType SCRelay_Signal_Veh_MoveDownSouth();
            break;
        case 'a':
        case 'A':
            VehDataType SCRelay_Signal_Veh_MoveLeftWest();
            break;
        case 'd':
        case 'D':
            VehDataType SCRelay_Signal_Veh_MoveRightEast();
            break;
        case '1':           //Scan *needs to have printf
            if (radiusBoolOne) {
                char SCRelay_Signal_Veh_ScanSouthWest(VehDataType *vdt);
            } else {
                char SCRelay_Signal_Veh_ScanSouthWest2(VehDataType *vdt);
            }
            break;
        case '2':
            if (radiusBoolOne) {
                char SCRelay_Signal_Veh_ScanSouth(VehDataType *vdt);
            } else {
                char SCRelay_Signal_Veh_ScanSouth2(VehDataType *vdt);
            }
            break;               
        case '3':
            if (radiusBoolOne) {
                char SCRelay_Signal_Veh_ScanSouthEast(VehDataType *vdt);
            } else {
                char SCRelay_Signal_Veh_ScanSouthEast2(VehDataType *vdt);
            }
            break;
        case '4':
            if (radiusBoolOne) {
                char SCRelay_Signal_Veh_ScanWest(VehDataType *vdt);
            } else {
                char SCRelay_Signal_Veh_ScanWest2(VehDataType *vdt);
            }        
            break;
        case '6':
            if (radiusBoolOne) {
                char SCRelay_Signal_Veh_ScanEast(VehDataType *vdt);
            } else {
                char SCRelay_Signal_Veh_ScanEast2(VehDataType *vdt);
            }
            break;
        case '7':
            if (radiusBoolOne) {
                char SCRelay_Signal_Veh_ScanNorthWest(VehDataType *vdt);
            } else {
                char SCRelay_Signal_Veh_ScanNorthWest2(VehDataType *vdt);
            }
            break;
        case '8':
            if (radiusBoolOne) {
                char SCRelay_Signal_Veh_ScanNorth (VehDataType * vdt);
            } else {
                char SCRelay_Signal_Veh_ScanNorth2 (VehDataType * vdt);
            }
            break;
        case '9':
            if (radiusBoolOne) {
                char SCRelay_Signal_Veh_ScanNorthEast(VehDataType *vdt);
            } else {
                char SCRelay_Signal_Veh_ScanNorthEast2(VehDataType *vdt);
            }
            break;         
        case 't':
        case 'T':           //Scan Radius
            radiusBoolOne = !radiusBoolOne;
            //need to have a printf use if else instead
            break;
        case 'i':
        case 'I':           //energyShield
            VehDataType SCRelay_Signal_Veh_AllocateToShield (long amountOfEnergy);
            //this needs to have error checking
            break;
        case 'z':
        case 'Z':           //vehicleStatus
            // code block
            break;
        case 'c':
        case 'C':           //Map o terrain scan
            // code block
            break;
        case 'x':
        case 'X':           //Map o terrain travelled
            // code block
            break;
        case 'q':
        case 'Q':           //QUIT
            // code block
            break;              
        case 'v':
        case 'V':           //Intruction
            // code block
            break; 
        default:            //Error handle
            // code block
        }
    }
    return 0;
}