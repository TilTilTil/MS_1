#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "SatComRelay.h"
#include "helpers.h"
#include <errno.h>
#include <limits.h>

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

// long getValidLongHelper() {
//     char digit[64];  
//     fgets(digit, sizeof(digit), stdin);
//     long num = strtold(digit, NULL);
//     return num;
// }

static long readNonNegative(long maxValue) {
    char buf[64];

    while (1) {
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            printf("\nInput closed (EOF). Exiting.\n");
            exit(1);
        }

        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') {
            buf[--len] = '\0';
        } else if (!feof(stdin)) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            printf("Input too long. Enter a whole number from 0 to %ld: ", maxValue);
            continue;
        }

        char *s = buf;
        while (isspace((unsigned char)*s)) s++;
        char *end = s + strlen(s);
        while (end > s && isspace((unsigned char)end[-1])) end--;
        *end = '\0';

        if (*s == '\0') {
            printf("Nothing entered. Enter a whole number from 0 to %ld: ", maxValue);
            continue;
        }

        bool digitsOnly = true;
        for (const char *p = s; *p != '\0'; p++) {
            if (!isdigit((unsigned char)*p)) { digitsOnly = false; break; }
        }
        if (!digitsOnly) {
            printf("Invalid input. Use digits only, no letters, spaces, signs or symbols (0 to %ld): ", maxValue);
            continue;
        }

        errno = 0;
        long value = strtol(s, NULL, 10);
        if (errno == ERANGE || value > maxValue) {
            printf("Number too large. Enter a whole number from 0 to %ld: ", maxValue);
            continue;
        }
        return value;
    }
}

long getValidLongHelper(void) {
    return readNonNegative(LONG_MAX);
}

int getValidIntHelper(void) {
    return (int)readNonNegative(INT_MAX);
}

void delay(int number_of_seconds)
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