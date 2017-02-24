#include "midterm.h"
#include "Common_Drive_data.h"

void driveRobot(wheel_values values) {
    if(getWallDistance() > 15)
    drive_speed(values.left, values.right);
    else drive_speed(0,0), wall_detected = 1;
}

void storeMotion(wheel_values values, motionStack stack) {
    static clock_t last_call = 0;
    clock_t current_call = clock();
    // initializing last_call
    if(!last_call) last_call = current_call;

    wheel_values emtyval;
    emtyval.left = 0;
    emtyval.right = 0;
    motion newMotion;
    if(!wall_detected) newMotion.values = values;
    else newMotion.values = emtyval;
    newMotion.time = current_call-last_call;

    motionStackPlate newStack = malloc(sizeof(amotionStackPlate));

    newStack->motion = newMotion;
    newStack->prev = stack->head;
    stack->head = newStack;
    last_call = current_call;
    return;
}

void doMotion(motion Motion){
    int leftDist = Motion.time* Motion.values.left;
    int rightDist = Motion.time*Motion.values.right;
    drive_goto(-1*leftDist, -1*rightDist);
}

void undo(motionStack stack) {
    motionStackPlate head = stack->head;
    while(head != NULL) {
        doMotion(head->motion);
        motionStackPlate current = head;
        head = head->prev;
        free(head);
    }
}

void followWall() {
    motionStack stack = malloc(sizeof(amotionStack));
    stack->head = NULL;
    while(!wall_detected) {
        executePID();
        wheel_values values = getOutput();
        driveRobot(values);
        storeMotion(values, stack);
    }
    undo(stack);
}



int main() {
    initialize(getSensorValues, getWallDistance);
    followWall();
}