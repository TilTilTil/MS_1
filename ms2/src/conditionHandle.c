
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "vehControl.h"
#include "SatComRelay.h"
#include "autoControl.h"
#include "config.h"
#include "condtionHandle.h"
#include "helpers.h"

static bool eVisitedMap[HEIGHT][WIDTH];
static int  eVisitedCount = 0;
 


static void stepDir(AutoState *a, PosType axis) {
    char next = a->scanMap[a->vehPos->y + axis.y][a->vehPos->x + axis.x];
    if(a->configOption.opt13 >= 0){delay(a->configOption.opt13);}
    *a->veh = SCRelay_Signal_Veh_AllocateToShield(a->costLog[next].shield);
    moveHelper(a->veh, a->vehPos, axis, a->costLog, a->scanMap, a->exploreMap, a->moveCounter, a->configOption);
}

static int sign(int v) { return (v > 0) - (v < 0); }

static bool goToE(AutoState *a) {
    for (size_t i = 0; i < sizeof(ALLAXIS) / sizeof(ALLAXIS[0]); i++) {
        PosType path;
        PosType steps[4];   //highest steps in a path is 4
        int num = 0;
        if (a->scanMap[a->vehPos->y + ALLAXIS[i].y][a->vehPos->x + ALLAXIS[i].x] == END_POINT && !eVisitedMap[a->vehPos->y + ALLAXIS[i].y][a->vehPos->x + ALLAXIS[i].x]) {
            path = ALLAXIS[i];
        } else if (a->scanMap[a->vehPos->y + ALLAXIS[i].y * 2][a->vehPos->x + ALLAXIS[i].x * 2] == END_POINT && !eVisitedMap[a->vehPos->y + ALLAXIS[i].y * 2][a->vehPos->x + ALLAXIS[i].x * 2]) {
            path = (PosType){ ALLAXIS[i].x * 2, ALLAXIS[i].y * 2 };
        } else {
            continue;
        }

        for (int i = 0; i < abs(path.y); i++) steps[num++] = (PosType){ 0, sign(path.y) };
        for (int i = 0; i < abs(path.x); i++) steps[num++] = (PosType){ sign(path.x), 0 };

        for (int i = 0; i < num; i++) {
            stepDir(a, steps[i]);
        }
        for (int i = num - 1; i >= 0; i--) {
            stepDir(a, (PosType){ -steps[i].x, -steps[i].y });
        }
        eVisitedMap[a->vehPos->y + path.y][a->vehPos->x + path.x] = true;
        eVisitedCount++;
        return true;
    }
    return false;
}

void tryVisitE(AutoState *a) {
    if (a->GGNOOBS) return;
    if (a->missionType == 3 || a->missionType == 5) {
        goToE(a);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// i'm too lazy to make a new file.

//mission
static int countTarget(char map[HEIGHT][WIDTH], char value) {   //super shit way to do it but it works lol
    int count = 0;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (map[y][x] == value) {
                count++;
            }
        }
    }
    return count;
}

static bool stopAndFlame( AutoState *a) { 
    (void)a; 
    printf("\nWait this mission doesn't need auto! Its Casual\n");
    return true; 
}

static bool neverStop( AutoState *a) { 
    (void)a; return false; 
}

static bool countoneE( AutoState *a) {
    return countTarget(a->scanMap, END_POINT) >= 1;
}

static bool countE( AutoState *a) {
    return countTarget(a->scanMap, END_POINT) >= NUM_ENDS;
}

static bool visitedOneE( AutoState *a) {
    (void)a;
    return eVisitedCount >= 1;
}
 
static bool visitedAllE( AutoState *a) {
    (void)a;
    return eVisitedCount >= NUM_ENDS;
}

static bool countDanger( AutoState *a) {
    return countTarget(a->scanMap, DANGER_POINT) >= NUM_DANGER;
}

//pauseCondition
static bool checkEnergy( AutoState *a) {
    return a->veh->currEnergy < a->configOption.opt14;
}

static void addEnergyLimit( AutoState *a) {
    printf("\nI can't help you recover energy, Unlucky! Would have let you gamble to get some energy if somebody....\n");
    printf("I can help you increase your limit though\n");
    printf("Your current energy: %ld\n", a->veh->currEnergy);
    printf("Your current limit: %ld\n", a->configOption.opt14);
    a->configOption.opt14 = getValidLongHelper();
}

static bool checkShield( AutoState *a) {
    return a->veh->currShieldEnergy < a->configOption.opt15;
}

static void recoverShield( AutoState *a) {
    printf("\nAllocate some shield then!\n");
    printf("Your current shield: %ld\n", a->veh->currShieldEnergy);
    printf("Your current limit: %ld\n", a->configOption.opt15);
    *a->veh = SCRelay_Signal_Veh_AllocateToShield (getValidLongHelper());
}

static bool checkMove( AutoState *a) {
    return *a->moveCounter > a->configOption.opt16;
}

static void addMoreMoves( AutoState *a) {
    printf("\nIncrease your limit then!\n");
    printf("Current steps --> %d\n", *a->moveCounter);
    printf("Your current limit: %d\n", a->configOption.opt16);
    a->configOption.opt16 = getValidIntHelper();
}

static bool checkScan( AutoState *a) {
    return *a->scanCounter > a->configOption.opt17;
}

static void addMoreScans( AutoState *a) {
    printf("\nIncrease your limit then!\n");
    printf("Current scans --> %d\n", *a->scanCounter);
    printf("Your current limit: %d\n", a->configOption.opt17);
    a->configOption.opt17 = getValidIntHelper();
}

static const Condition missionStatement[] = {
    [0] = { 1, stopAndFlame, NULL },  // 1. Casual Explore bruh bruh lowkey should just ignore but.
    [1] = { 1, countoneE,    NULL },  // 2. Find 1 End Point DONE
    [2] = { 1, visitedOneE,    NULL },  // 3. Reach 1 End Point TODO DONE
    [3] = { 1, countE,    NULL },  // 4. Find All End Points DONE
    [4] = { 1, visitedAllE, NULL },  // 5. Reach ALL End Points TODO DONE
    [5] = { 1, countDanger, NULL },  // 6. Find ALL Danger Points DONE
    [6] = { 1, neverStop, NULL },  // 7. Find Map Boundaries TODO DONE
    [7] = { 1, neverStop, NULL },  // 8. Map Everything DONE
};

static const Condition pauseConditions[] = {
    { 1, checkEnergy, addEnergyLimit},
    { 1, checkShield, recoverShield},
    { 1, checkMove, addMoreMoves},
    { 1, checkScan, addMoreScans},
};

bool shouldStop(AutoState *a) {
    if (a->GGNOOBS) return true;
    int m = a->missionType - 1;
    size_t n = sizeof(missionStatement) / sizeof(missionStatement[0]);
    if (m >= 0 && (size_t)m < n && missionStatement[m].check(a)) {
        a->GGNOOBS = true;
        return true;
    }
    for (size_t i = 0; i < sizeof(pauseConditions) / sizeof(pauseConditions[0]); i++) {
        if (pauseConditions[i].check(a)) {
            if (getValidChar("Continue auto-pilot? (y/n): \n", "YN") == 'N') {
                a->GGNOOBS = true;
                return true;
            } else {
                pauseConditions[i].helpMe(a);
            }
        }
    }
    return false;
}