#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include "SatComRelay.h"

void scanMap() {
    printf("\n")
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

int scanInt(){
    char digit[10];  
    long int num;     
    fgets(digit, sizeof(digit), stdin);
    num = atoi(digit);
    return num;
}

VehDataType initVehicle() {
    char scenarioFile[256];
    char answer ='\0'; //cannot start with unknown 
    bool isFileEncrypted;
    bool randomizeStartPosition;
    int missionType;

    printf("\n\nPlease enter the scenario file's name:\t");
    fgets(scenarioFile, sizeof(scenarioFile), stdin);
    scenarioFile[strcspn(scenarioFile, "\n")] = '\0';   //need remove \n

    printf("\n\nIs the scenario file encrypted?\t(y/n): ");
    answer = scanfErrorHandle();

    if (answer == 'y' || answer == 'Y')
        isFileEncrypted = true;
    else
        isFileEncrypted = false;

    printf("\n\nDo you wish to randomize start position?\t(y/n): ");
    answer = scanfErrorHandle();

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
    missionType = scanInt();

    printf("\n\nfilenname : %s, isFileEncrypted : %d, randomizeStartPosition : %d\n", scenarioFile, isFileEncrypted, randomizeStartPosition);

    return SCRelay_Signal_Veh_Init(
        scenarioFile,
        isFileEncrypted,
        randomizeStartPosition,
        missionType
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
    showUsageInstructions();
    char answer[10];
    bool radiusBoolOne = true;          //default use case is 1
    while (answer[0] != 'q' && answer[0] != 'Q')
    {
        printf("\n\nEnter your command (q to quit) :");
        fgets(answer, sizeof(answer), stdin);   //fix the error handling one day
        
        switch (answer[0]) {
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
        case '1':    
            if (radiusBoolOne) {
                printf("Returned (scanned) terrain symbol is : %c", SCRelay_Signal_Veh_ScanSouthWest(&vehicleData));
            } else {
                printf("Returned (scanned) terrain symbol is : %c", SCRelay_Signal_Veh_ScanSouthWest2(&vehicleData));
            }
            break;

        case '2':         
            if (radiusBoolOne) {
                printf("Returned (scanned) terrain symbol is : %c", SCRelay_Signal_Veh_ScanSouth(&vehicleData));
            } else {
                printf("Returned (scanned) terrain symbol is : %c", SCRelay_Signal_Veh_ScanSouth2(&vehicleData));
            }
            break;

        case '3':     
            if (radiusBoolOne) {
                printf("Returned (scanned) terrain symbol is : %c", SCRelay_Signal_Veh_ScanSouthEast(&vehicleData));
            } else {
                printf("Returned (scanned) terrain symbol is : %c", SCRelay_Signal_Veh_ScanSouthEast2(&vehicleData));
            }
            break;

        case '4':         
            if (radiusBoolOne) {
                printf("Returned (scanned) terrain symbol is : %c", SCRelay_Signal_Veh_ScanWest(&vehicleData));
            } else {
                printf("Returned (scanned) terrain symbol is : %c", SCRelay_Signal_Veh_ScanWest2(&vehicleData));
            }
            break;

        case '5':        
            printf("Current vehicle position.\n");
            break;

        case '6':          
            if (radiusBoolOne) {
                printf("Returned (scanned) terrain symbol is : %c", SCRelay_Signal_Veh_ScanEast(&vehicleData));
            } else {
                printf("Returned (scanned) terrain symbol is : %c", SCRelay_Signal_Veh_ScanEast2(&vehicleData));
            }
            break;

        case '7':           
            if (radiusBoolOne) {
                printf("Returned (scanned) terrain symbol is : %c", SCRelay_Signal_Veh_ScanNorthWest(&vehicleData));
            } else {
                printf("Returned (scanned) terrain symbol is : %c", SCRelay_Signal_Veh_ScanNorthWest2(&vehicleData));
            }
            break;

        case '8':          
            if (radiusBoolOne) {
                printf("Returned (scanned) terrain symbol is : %c", SCRelay_Signal_Veh_ScanNorth(&vehicleData));
            } else {
                printf("Returned (scanned) terrain symbol is : %c", SCRelay_Signal_Veh_ScanNorth2(&vehicleData));
            }
            break;

        case '9':           
            if (radiusBoolOne) {
                printf("Returned (scanned) terrain symbol is : %c", SCRelay_Signal_Veh_ScanNorthEast(&vehicleData));
            } else {
                printf("Returned (scanned) terrain symbol is : %c", SCRelay_Signal_Veh_ScanNorthEast2(&vehicleData));
            }
            break;     
        case 't':
        case 'T':           //Scan Radius
            radiusBoolOne = !radiusBoolOne;
            //need to have a printf use if else instead
            break;
        case 'i':
        case 'I':           //energyShield
        {                   //block
            double amountOfEnergy;      //change to double to handle decimel error handling
            printf("\nEnter amt. of energy to increase / allocate to shield ( 1 to %ld ) :",vehicleData.currEnergy);
            if (scanf("%lf", &amountOfEnergy) != 1)
            {
                printf("\nThat's not a number!\n");
            }
            else if(floor(amountOfEnergy) != amountOfEnergy || amountOfEnergy <= 0) {
                printf("\nError: Energy Allocation must be in whole numbers!\n");
            }
            else if (amountOfEnergy > vehicleData.currEnergy) {
                printf("\nError: Insufficient energy reserves, Try reducing the quant allocated to shield!\n");
            }
            else {
            SCRelay_Signal_Veh_AllocateToShield (amountOfEnergy);
            }
            break;
        }
        case 'z':
        case 'Z':           //vehicleStatus
            printf("\nVehicle Status / Statistics :\n");
            printf("\nInitial Enenrgy: %ld, Total Energy Used: %ld, Energy left: %ld, Shield Energy Left: %ld\n",vehicleData.initialEnergy,vehicleData.totEnergyUsed,vehicleData.currEnergy,vehicleData.currShieldEnergy);
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
            showUsageInstructions();
            break; 
        default:            //Error handle
            // code block
        }
    }
    return 0;
}