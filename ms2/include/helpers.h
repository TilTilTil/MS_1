#ifndef HELPERS_H
#define HELPERS_H

#include "SatComRelay.h"

#define NORTH     ((PosType){.x =  0, .y = -1})
#define SOUTH     ((PosType){.x =  0, .y =  1})
#define EAST      ((PosType){.x =  1, .y =  0})
#define WEST      ((PosType){.x = -1, .y =  0})

#define NORTHWEST ((PosType){.x = -1, .y = -1})
#define NORTHEAST ((PosType){.x =  1, .y = -1})
#define SOUTHWEST ((PosType){.x = -1, .y =  1})
#define SOUTHEAST ((PosType){.x =  1, .y =  1})

#define ALLAXIS ((PosType[8]){NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST, WEST, NORTHWEST})

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
    bool autoOn;
}Option;

char getValidChar(const char *prompt, const char *validChars);

long getValidLongHelper();

void delay(long number_of_seconds);

void statusHelper(VehDataType *vehicleData, Option configOption );

#endif