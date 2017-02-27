#include "midterm.h"
#include "Common_Drive_data.h"

void driveRobot(wheel_values values) {
    if(!wall_detected) {
    if(getWallDistance() > MIN_DISTANCE)
    drive_speed(values.left, values.right);
    else drive_speed(0,0), wall_detected = 1;
    }
    else {
        drive_speed(values.left, values.right);
    }
}

motion storeMotion(wheel_values values, motion stack){
    static int left = -1,right = -1;
    if(values.left == left && values.right == right) return stack;
    else {left = values.left; right = values.right;}
    time_t current_call_time = time(&current_call_time);
    time_t time = current_call_time - last_call_value;
    last_call_value = current_call_time;


    motion new_motion = malloc(sizeof(__motion__));
    new_motion->prev = stack;
    new_motion->values = values;
    new_motion->time = time;
    printf("Recording %dms of %d %d\n", time, new_motion->values.left, new_motion->values.right);

    return new_motion;
}


time_t time_remaining = 0;
time_t start_time = 0;
void undoMotion(motion Motion){
    time_t time_seconds = Motion->time;

    printf("Undoing %lds of %d %d\n", time_seconds, Motion->values.left, Motion->values.right);

    wheel_values result;
    result.left = Motion->values.right;
    result.right = Motion->values.left;
    start_time = time(NULL);
    time_remaining = (int)time_seconds;
    driveRobot(result);
}
void undo(motion stack) {
    static int i = 0;
    while(stack) {
        i++;
        printf("%ld, %ld", start_time, time_remaining);
        time_t time_to_pause = (time(NULL) - start_time - time_remaining);
        printf("[%d]: Thinking about pausing for  %ld\n", time_to_pause);
        if(i != 1)
        if(time_to_pause > 0)
            pause(time_to_pause * 1000);
        undoMotion(stack);
        stack = stack->prev;
    }
}

void followWall() {
    motion stack = NULL;
    last_call_value = time(NULL);
    while(!wall_detected) {
        executePID();
        wheel_values values = getOutput();
        driveRobot(values);
        stack = storeMotion(values, stack);
    }
    drive_speed(0, 0);
    pause(10);
    safeZeroTurn(180);

    drive_speed(0, 0);
    undo(stack);
}

int main() {
    initialize(getSensorValues, getWallDistance);
    followWall();
}