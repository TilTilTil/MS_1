#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "SatComRelay.h"
#include "mapMake.h"

VehDataType initVehicle() {
    return SCRelay_Signal_Veh_Init(
        "scenario/scenario.dat",
        'y',
        'y',
        '8'
    );
}

int main() {
    // VehDataType vehicleData = initVehicle();
    // SCRelay_Signal_Veh_Init(
    //     "scenario/scenario.dat",
    //     'y',
    //     'y',
    //     '8'
    // );
    // VehDataType Veh;
    // Veh = SCRelay_Signal_Veh_AllocateToShield(200000);
    // printf("Initial Energy       : %ld\n", Veh.initialEnergy);
    // printf("Current Energy       : %ld\n", Veh.currEnergy);
    // printf("Current Shield Energy: %ld\n", Veh.currShieldEnergy);
    // printf("Total Energy Used    : %ld\n", Veh.totEnergyUsed);
    // printf("Total Shield Used    : %ld\n", Veh.totShieldEnergyUsed);
    // printf("\n");
    // Veh = SCRelay_Signal_Veh_MoveUpNorth();
    // printf("Initial Energy       : %ld\n", Veh.initialEnergy);
    // printf("Current Energy       : %ld\n", Veh.currEnergy);
    // printf("Current Shield Energy: %ld\n", Veh.currShieldEnergy);
    // printf("Total Energy Used    : %ld\n", Veh.totEnergyUsed);
    // printf("Total Shield Used    : %ld\n", Veh.totShieldEnergyUsed);

    // char scanMap[HEIGHT][WIDTH];
    // char exploreMap[HEIGHT][WIDTH];
    VehPosType vehPos;
    vehPos.x = WIDTH/2;
    vehPos.y = HEIGHT/2;
    printf("TEST");
    printf("%d",vehPos.x);
    vehPos.x = vehPos.x + 1;
    printf("%d",vehPos.x);
}