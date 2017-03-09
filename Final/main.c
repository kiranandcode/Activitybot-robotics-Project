//
// Created by gopia on 07/03/2017.
//

#include "main.h"
#include "simulator.h"
double x = 0.0;
double y = 0.0;
double angle = 0.0;

int main() {
    int i = 0;
    simulator_startNewSmokeTrail();
    autotune();
    while(i < 2) {
    i++;
        printf("%d\n", i);
        //tuplePtr leftright = getSensorValues();
        //printf("%d, %d\n", leftright->left, leftright->right);
        //free(leftright);
        //printf("%d\n",getConsistentResults(getWallDistance));
        //
        //drive_goForward(10,5);
        //driveforconsistent();
        //readjust();
        followPath(travelToDeadEnd());
    }
    return 0;
}
