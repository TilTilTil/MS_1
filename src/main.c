#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include "cleaner.h"

#include "SatComRelay.h"
#include "mapMake.h"

void scanMap() {
    printf("\n");
}


int canMove(char terrain)
{
    return terrain != '#';
}



char scanfErrorHandle(void)
{
    char answer[10];

    fgets(answer, sizeof(answer), stdin);

    // Remove the newline
    answer[strcspn(answer, "\n")] = '\0';

    // Check that exactly ONE character was entered
    if (strlen(answer) != 1)
    {
        printf("Please enter only one character.\n");
        return '\0';
    }

    return answer[0];
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
 
while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N')
{
    printf("Invalid input. Please enter y or n: ");

    answer = scanfErrorHandle();
}

if (answer == 'y' || answer == 'Y')
{
     isFileEncrypted = true;
     printf("encrypted");
}
else if (answer == 'n' || answer == 'N')
{
    isFileEncrypted = false;
    printf("not encrypted");
}

    

printf("\n\nDo you wish to randomize start position?\t(y/n): ");

answer = scanfErrorHandle();

while (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N')
{
    printf("Invalid input. Please enter y or n: ");

    answer = scanfErrorHandle();
}

if (answer == 'y' || answer == 'Y')
{
    randomizeStartPosition = true;
    printf("randomised");
}
else if (answer == 'n' || answer == 'N')
{
    randomizeStartPosition = false;
    printf("not random");
}

    //VehDataType result = {0};

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

char digit[10];
char *endptr;
int num;

while (1)
{
    fgets(digit, sizeof(digit), stdin);

    num = strtol(digit, &endptr, 10);

    // Check if the user entered something that isn't a valid number
    if (endptr == digit || (*endptr != '\n' && *endptr != '\0'))
    {
        printf("kill yourself. Please enter a number from 1 to 8: ");
        continue;
    }

    // Check if number is outside 1-8
    if (num < 1 || num > 8)
    {
        printf("kill yourself. Please enter a number from 1 to 8: ");
        continue;
    }

    // Valid number
    break;
}

missionType = num;

printf("\n\nfilenname : %s, isFileEncrypted : %d, randomizeStartPosition : %d\n",
       scenarioFile, isFileEncrypted, randomizeStartPosition);

return SCRelay_Signal_Veh_Init(
    scenarioFile,
    isFileEncrypted,
    randomizeStartPosition,
    missionType
);
}

// VehDataType initVehicle() {
//     return SCRelay_Signal_Veh_Init(
//         "scenario/scenario.dat",
//         'y',
//         'n',
//         '8'
//     );
// }

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
        cleanInput(answer);
        answer[strcspn(answer, "\n")] = '\0';

		if (strlen(answer) != 1)
{
   			 printf("Invalid command. Please enter exactly one character.\n");
    	continue;
}

 
        switch (answer[0]) {
        
        
        
        
        case 'w':
		{
    		char north = SCRelay_Signal_Veh_ScanNorth(&vehicleData);

   			 if (canMove(north))
   			 {
       			vehicleData = SCRelay_Signal_Veh_MoveUpNorth();
        		vehPos.y--;
        		printf("\nInitial Energy: %ld, Total Energy Used: %ld, Energy left: %ld, Shield Energy Left: %ld\n",vehicleData.initialEnergy,vehicleData.totEnergyUsed,vehicleData.currEnergy,vehicleData.currShieldEnergy);
}
    		else
   {
        printf("Cannot move North! Border detected.\n");
    }

    break;
}
        
        case 's':
			char south = SCRelay_Signal_Veh_ScanSouth(&vehicleData);
			
			 if (canMove(south))
            {
           	 vehicleData = SCRelay_Signal_Veh_MoveDownSouth();
            	vehPos.y = vehPos.y + 1;
            	printf("\nInitial Energy: %ld, Total Energy Used: %ld, Energy left: %ld, Shield Energy Left: %ld\n",vehicleData.initialEnergy,vehicleData.totEnergyUsed,vehicleData.currEnergy,vehicleData.currShieldEnergy);
            }
            else
            { 
            	printf("Cannot move South! Border detected.\n");
            }
            
            
            break;
        case 'a':
			char west = SCRelay_Signal_Veh_ScanWest(&vehicleData);
			if (canMove(west))
			{
           	 vehicleData = SCRelay_Signal_Veh_MoveLeftWest();
           	 vehPos.x = vehPos.x - 1;
             printf("\nInitial Energy: %ld, Total Energy Used: %ld, Energy left: %ld, Shield Energy Left: %ld\n",vehicleData.initialEnergy,vehicleData.totEnergyUsed,vehicleData.currEnergy,vehicleData.currShieldEnergy);
             }
             
             else
             {  
             	printf("Cannot move West! Border detected.\n");
             }
             	
            break;
        case 'd':
			char east = SCRelay_Signal_Veh_ScanEast(&vehicleData);
			if (canMove(east))
			{
            	vehicleData = SCRelay_Signal_Veh_MoveRightEast();
            	vehPos.x = vehPos.x + 1;
            	printf("\nInitial Energy: %ld, Total Energy Used: %ld, Energy left: %ld, Shield Energy Left: %ld\n",vehicleData.initialEnergy,vehicleData.totEnergyUsed,vehicleData.currEnergy,vehicleData.currShieldEnergy);
 }
            else
            {
            	printf("Cannot move West! Border detected.\n");
             }
            
            break;
            //value 1 stores the #,w,f
            // 0,1 is N 1,0 is E
            
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
                printMapScan(scanMap, vehPos, 0, -1, value1);
            } else {
                char value2 = SCRelay_Signal_Veh_ScanSouth2(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value2);
                printMapScan(scanMap, vehPos, 0, -2, value2);
            }
            break;

        case '3':     
            if (radiusBoolOne) {
                char value1 = SCRelay_Signal_Veh_ScanSouthEast(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value1);
                printMapScan(scanMap, vehPos, 1, -1, value1);
            } else {
                char value2 = SCRelay_Signal_Veh_ScanSouthEast2(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value2);
                printMapScan(scanMap, vehPos, 2, -2, value2);
            }
            break;

        case '4':         
            if (radiusBoolOne) {
                char value1 = SCRelay_Signal_Veh_ScanWest(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value1);
                printMapScan(scanMap, vehPos, -1, 0, value1);
            } else {
                char value2 = SCRelay_Signal_Veh_ScanWest2(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value2);
                printMapScan(scanMap, vehPos, -2, 0, value2);
            }
            break;

        case '6':          
            if (radiusBoolOne) {
                char value1 = SCRelay_Signal_Veh_ScanEast(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value1);
                printMapScan(scanMap, vehPos, 1, 0, value1);
            } else {
                char value2 = SCRelay_Signal_Veh_ScanEast2(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value2);
                printMapScan(scanMap, vehPos, 2, 0, value2);
            }
            break;

        case '7':           
            if (radiusBoolOne) {
                char value1 = SCRelay_Signal_Veh_ScanNorthWest(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value1);
                printMapScan(scanMap, vehPos, -1, 1, value1);
            } else {
                char value2 = SCRelay_Signal_Veh_ScanNorthWest2(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value2);
                printMapScan(scanMap, vehPos, -2, 2, value2);
            }
            break;

        case '8':          
            if (radiusBoolOne) {
                char value1 = SCRelay_Signal_Veh_ScanNorth(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value1);
                printMapScan(scanMap, vehPos, 0, 1, value1);
            } else {
                char value2 = SCRelay_Signal_Veh_ScanNorth2(&vehicleData);
                printf("Returned (scanned) terrain symbol is : %c", value2);
                printMapScan(scanMap, vehPos, 0, 2, value2);
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
                 //energyShield
            printf("\n allocate energy to shield ( 1 to %ld ) :",vehicleData.currEnergy);
            char digit[10];  
            long num;     
            fgets(digit, sizeof(digit), stdin);
            num = strtold(digit, NULL);
            vehicleData = SCRelay_Signal_Veh_AllocateToShield (num);
            break;
            
            
     


        
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
