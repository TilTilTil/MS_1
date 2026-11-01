#ifndef CONDITIONHANDLE_H
#define CONDITIONHANDLE_H

#include "autoControl.h"

typedef bool (*exitOrPause)( AutoState *);
typedef void (*Unpause)( AutoState *);

typedef struct { 
    const char missionType; 
    exitOrPause check; 
    Unpause helpMe;
} Condition;

bool shouldStop(AutoState *a);

void tryVisitE(AutoState *a);
#endif