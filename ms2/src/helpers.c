#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "SatComRelay.h"
#include "helpers.h"

char getValidChar(const char *prompt, const char *validChars) {
    char input[16];

    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            exit(1);
        }
        if (input[0] != '\n' && input[1] == '\n') {
            char choice = toupper((unsigned char)input[0]);
            if (strchr(validChars, choice) != NULL) {
                return choice;
            }
        }
        printf("Invalid input. Try again!");
    }
}

long getValidLongHelper() {
    char digit[64];  
    fgets(digit, sizeof(digit), stdin);
    long num = strtold(digit, NULL);
    return num;
}

void delay(long number_of_seconds)
{
    // Storing start time
    clock_t start_time = clock();

    // Looping till required time is not achieved
    while (clock() - start_time < number_of_seconds * CLOCKS_PER_SEC)
        ;
}

void statusHelper(VehDataType *vehicleData, Option configOption) {
    printf("\nInitial Energy: %ld, Total Energy Used: %ld, Energy left: %ld, Shield Energy Left: %ld\n",vehicleData->initialEnergy,vehicleData->totEnergyUsed,vehicleData->currEnergy,vehicleData->currShieldEnergy);
    if(configOption.opt7 >= 0) { delay(configOption.opt7); };
}