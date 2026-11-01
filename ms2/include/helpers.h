#ifndef HELPERS_H
#define HELPERS_H

#include "SatComRelay.h"

typedef struct {
    bool opt1;
    bool opt2;
    bool opt3;
    bool opt4;
    bool opt5;
    bool opt6;
    long opt7;
    long opt8;
    long opt9;
    long opt10;
    long opt11;
    long opt12;
    long opt13;
    long opt14;
    long opt15;
    int opt16;
    int opt17;
}Option;

char getValidChar(const char *prompt, const char *validChars);

long getValidLongHelper();

void delay(long number_of_seconds);

void statusHelper(VehDataType *vehicleData, Option configOption );

#endif