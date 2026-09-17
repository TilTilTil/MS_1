#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "SatComRelay.h"
#include "mapMake.h"

void scanMap() {
    printf("\n");
}
char scanfErrorHandle() {   //trying to use fgets instead of scanf
    char answer[10];
    fgets(answer, sizeof(answer), stdin);
    if(isalpha(answer[0])) {    //checks if its a number,special character or even a spacebar
        // printf("%c",answer[0]);
        return answer[0];
    }
    else if(answer[1] != '\n') { //check if user keyed in multiple inputs
        printf("Key in only one input");
        return '\0';
    }
    else {
        printf("invalid input!");
        return '\0';
    }
}

// VehDataType initVehicle() {
//     char scenarioFile[256];
//     char answer ='\0'; //cannot start with unknown 
//     bool isFileEncrypted;
//     bool randomizeStartPosition;
//     int missionType;

//     printf("\n\nPlease enter the scenario file's name:\t");
//     fgets(scenarioFile, sizeof(scenarioFile), stdin);
//     scenarioFile[strcspn(scenarioFile, "\n")] = '\0';   //need remove \n

//     printf("\n\nIs the scenario file encrypted?\t(y/n): ");
//     answer = scanfErrorHandle();

//     if (answer == 'y' || answer == 'Y')
//         isFileEncrypted = true;
//     else
//         isFileEncrypted = false;

//     printf("\n\nDo you wish to randomize start position?\t(y/n): ");
//     answer = scanfErrorHandle();

//     if (answer == 'y' || answer == 'Y')
//         randomizeStartPosition = true;
//     else
//         randomizeStartPosition = false;

//     printf("\nSelect mission:\n");
//     printf("1. Casual Explore\n");
//     printf("2. Find 1 End Point\n");
//     printf("3. Reach 1 End Point\n");
//     printf("4. Find All End Points\n");
//     printf("5. Reach ALL End Points\n");
//     printf("6. Find ALL Danger Points\n");
//     printf("7. Find Map Boundaries\n");
//     printf("8. Map Everything\n");

//     printf("\nEnter your choice (1-8): ");
//     char digit[10];  
//     int num;     
//     fgets(digit, sizeof(digit), stdin);
//     num = strtold(digit, NULL);
//     missionType = num;

//     printf("\n\nfilenname : %s, isFileEncrypted : %d, randomizeStartPosition : %d\n", scenarioFile, isFileEncrypted, randomizeStartPosition);

//     return SCRelay_Signal_Veh_Init(
//         scenarioFile,
//         isFileEncrypted,
//         randomizeStartPosition,
//         missionType
//     );
// }

VehDataType initVehicle() {
    return SCRelay_Signal_Veh_Init(
        "scenario/scenario.dat",
        'y',
        'y',
        '8'
    );
}

int showUsageInstructions(void) {
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
        answer = scanfErrorHandle();
    }
    return 0;
}

int main(void) {
    VehDataType vehicleData = initVehicle();
    char scanMap[HEIGHT][WIDTH];
    char exploreMap[HEIGHT][WIDTH];
    VehPosType vehPos;
    vehPos.x = WIDTH/2;
    vehPos.y = HEIGHT/2;
    mapMake(scanMap, vehPos);
    mapMake(exploreMap, vehPos);

    showUsageInstructions();
    char answer[10] = "";
    bool radiusBoolOne = true;          //default use case is 1
    while (answer[0] != 'q' && answer[0] != 'Q')
    {
        printf("\n\nEnter your command (q to quit) :");
        fgets(answer, sizeof(answer), stdin);   //fix the error handling one day
        
        switch (answer[0]) {
        case 'w':           
        case 'W':           //Movement
            vehicleData = SCRelay_Signal_Veh_MoveUpNorth();
            printf("%d",vehPos.x);
            vehPos.x = vehPos.x + 1;
            printf("%d",vehPos.x);
            printf("\nInitial Energy: %ld, Total Energy Used: %ld, Energy left: %ld, Shield Energy Left: %ld\n",vehicleData.initialEnergy,vehicleData.totEnergyUsed,vehicleData.currEnergy,vehicleData.currShieldEnergy);
            break;
        case 's':
        case 'S':
            vehicleData = SCRelay_Signal_Veh_MoveDownSouth();
            vehPos.x = vehPos.x - 1;
            printf("\nInitial Energy: %ld, Total Energy Used: %ld, Energy left: %ld, Shield Energy Left: %ld\n",vehicleData.initialEnergy,vehicleData.totEnergyUsed,vehicleData.currEnergy,vehicleData.currShieldEnergy);
            break;
        case 'a':
        case 'A':
            vehicleData = SCRelay_Signal_Veh_MoveLeftWest();
            vehPos.y = vehPos.y + 1;
            printf("\nInitial Energy: %ld, Total Energy Used: %ld, Energy left: %ld, Shield Energy Left: %ld\n",vehicleData.initialEnergy,vehicleData.totEnergyUsed,vehicleData.currEnergy,vehicleData.currShieldEnergy);
            break;
        case 'd':
        case 'D':
            vehicleData = SCRelay_Signal_Veh_MoveRightEast();
            vehPos.y = vehPos.y - 1;
            printf("\nInitial Energy: %ld, Total Energy Used: %ld, Energy left: %ld, Shield Energy Left: %ld\n",vehicleData.initialEnergy,vehicleData.totEnergyUsed,vehicleData.currEnergy,vehicleData.currShieldEnergy);
            break;
            
        case '1':    
            if (radiusBoolOne) {
                char value1 = SCRelay_Signal_Veh_ScanSouthWest(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value1);
                printMapScan(scanMap, vehPos, -1, -1, value1);
            } else {
                char value2 = SCRelay_Signal_Veh_ScanSouthWest2(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value2);
                printMapScan(scanMap, vehPos, -2, -2, value2);
            }
            break;

        case '2':         
            if (radiusBoolOne) {
                char value1 = SCRelay_Signal_Veh_ScanSouth(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value1);
                printMapScan(scanMap, vehPos, -1, 0, value1);
            } else {
                char value2 = SCRelay_Signal_Veh_ScanSouth2(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value2);
                printMapScan(scanMap, vehPos, -2, 0, value2);
            }
            break;

        case '3':     
            if (radiusBoolOne) {
                char value1 = SCRelay_Signal_Veh_ScanSouthEast(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value1);
                printMapScan(scanMap, vehPos, -1, 1, value1);
            } else {
                char value2 = SCRelay_Signal_Veh_ScanSouthEast2(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value2);
                printMapScan(scanMap, vehPos, -2, 2, value2);
            }
            break;

        case '4':         
            if (radiusBoolOne) {
                char value1 = SCRelay_Signal_Veh_ScanWest(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value1);
                printMapScan(scanMap, vehPos, 0, -1, value1);
            } else {
                char value2 = SCRelay_Signal_Veh_ScanWest2(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value2);
                printMapScan(scanMap, vehPos, 0, -2, value2);
            }
            break;

        case '6':          
            if (radiusBoolOne) {
                char value1 = SCRelay_Signal_Veh_ScanEast(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value1);
                printMapScan(scanMap, vehPos, 0, 1, value1);
            } else {
                char value2 = SCRelay_Signal_Veh_ScanEast2(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value2);
                printMapScan(scanMap, vehPos, 0, 2, value2);
            }
            break;

        case '7':           
            if (radiusBoolOne) {
                char value1 = SCRelay_Signal_Veh_ScanNorthWest(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value1);
                printMapScan(scanMap, vehPos, 1, -1, value1);
            } else {
                char value2 = SCRelay_Signal_Veh_ScanNorthWest2(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value2);
                printMapScan(scanMap, vehPos, 2, -2, value2);
            }
            break;

        case '8':          
            if (radiusBoolOne) {
                char value1 = SCRelay_Signal_Veh_ScanNorth(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value1);
                printMapScan(scanMap, vehPos, 1, 0, value1);
            } else {
                char value2 = SCRelay_Signal_Veh_ScanNorth2(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value2);
                printMapScan(scanMap, vehPos, 2, 0, value2);
            }
            break;

        case '9':           
            if (radiusBoolOne) {
                char value1 = SCRelay_Signal_Veh_ScanNorthEast(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value1);
                printMapScan(scanMap, vehPos, 1, 1, value1);
            } else {
                char value2 = SCRelay_Signal_Veh_ScanNorthEast2(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value2);
                printMapScan(scanMap, vehPos, 2, 2, value2);
            }
            break;

        case 't':
        case 'T':           //Scan Radius
            radiusBoolOne = !radiusBoolOne;
            //need to have a printf so use an if else instead
            break;
        case 'i':
        case 'I':           //energyShield
            printf("\nEnter amt. of energy to increase / allocate to shield ( 1 to %ld ) :",vehicleData.currEnergy);
            char digit[10];  
            long num;     
            fgets(digit, sizeof(digit), stdin);
            num = strtold(digit, NULL);
            vehicleData = SCRelay_Signal_Veh_AllocateToShield (num);
            // printf("\nEnter amt. of energy to increase / allocate to shield ( 1 to %ld ) :",vehicleData.currEnergy);
            // if (scanf("%lf", &amountOfEnergy) != 1)
            // {
            //     printf("\nThat's not a number!\n");
            // }
            // else if(floor(amountOfEnergy) != amountOfEnergy || amountOfEnergy <= 0) {
            //     printf("\nError: Energy Allocation must be in whole numbers!\n");
            // }
            // else if (amountOfEnergy > vehicleData.currEnergy) {
            //     printf("\nError: Insufficient energy reserves, Try reducing the quant allocated to shield!\n");
            // }
            // else {
            // SCRelay_Signal_Veh_AllocateToShield (amountOfEnergy);
            // }
            break;
        case 'z':
        case 'Z':           //vehicleStatus
            printf("\nVehicle Status / Statistics :\n");
            printf("\nInitial Energy: %ld, Total Energy Used: %ld, Energy left: %ld, Shield Energy Left: %ld\n",vehicleData.initialEnergy,vehicleData.totEnergyUsed,vehicleData.currEnergy,vehicleData.currShieldEnergy);
            break;
        case 'c':
        case 'C':           //Map o terrain scan
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
            showUsageInstructions();
            break; 
        default:            //Error handle
            // code block
        }
    }
    return 0;
}