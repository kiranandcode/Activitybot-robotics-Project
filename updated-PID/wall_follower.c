#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ping.h>

#include "simpletools.h" // Include simple tools
#include "abdrive.h"
#include "simpletext.h"
#include "cog.h"
#define  SENSITIVITY 100.0

typedef struct motion {
    clock_t time_taken;
    int left_wheel_speed;
    int right_wheel_speed;
    struct motion *prevMotion;
} motion_value, *Motion;

float TICKS_P_MM = 3.25;
float RADIUS = 52.9;

int DO = 22, CLK = 23, DI = 24, CS = 25;
double angle_turned, distance_moved;
volatile int irLeft;
volatile int irRight;
volatile int sensor_reading_completed = 0;
volatile int driveValue = 50;
double targetValue;
double inputValue, old_inputValue, min_outputValue, max_outputValue;
double old_errorValue;
double integralSum = 0;
double output = 50;
double ki, kp, kd;
clock_t lastTime = 0;
double timeInterval = 1000;
int pid_initialized = 0;
int pid_activated = 0;
volatile int running = 1;

int last_left_call, last_right_call;
clock_t last_call_time = 0;
Motion robotMotions = 0;

double leftCurrentValue = 0.0;
double rightCurrentValue = 0.0;

void safeZeroTurn(float angle_degrees) {

    //initialize angle_to_turn to 0 once.
    static float angle_to_turn = 0;

    int start_distance[2];
    int end_distance[2];

    //convert to radians - loss-less
    float desired_angle_radians = angle_degrees * 2.0 * PI / 360.0;

    //add this angle to angle to turn;
    angle_to_turn += desired_angle_radians;

    //calculate ticks to turn - lossy.
    int ticks_to_turn = RADIUS * angle_to_turn / TICKS_P_MM;

    //calculate exact loss in angle from conversion.
    float conversion_angle_loss = angle_to_turn - (ticks_to_turn*TICKS_P_MM/RADIUS);

    //actually move through the turn.
    drive_getTicks(start_distance, start_distance+1);
    drive_goto(ticks_to_turn, -1 * ticks_to_turn);
    drive_getTicks(end_distance, end_distance+1);

    //calculate the ticks actually turned.
    int ticks_turned = end_distance[0] - start_distance[0]; // + (-1) * (end_distance[1] - start_distance[1])); /2

    //calculate the loss in ticks.
    int delta_ticks = ticks_to_turn - ticks_turned;
    printf("Ticks to turn, Ticks turned: %d,%d\nLoss in ticks: %d\n",ticks_to_turn, ticks_turned, delta_ticks);

    //calculate loss in angle due to loss in ticks.
    float movement_angle_loss = delta_ticks * TICKS_P_MM / RADIUS;

    //store this loss for the next iteration.
    angle_to_turn = conversion_angle_loss + movement_angle_loss;
    printf("Angle-loss: %f\n", angle_to_turn);
    return;
}

void outputDataToSDCard() {
    sd_mount(DO, CLK, DI, CS);                  // Mount SD card
    FILE* fp = fopen("team_3-Result.txt", "w");          // Open a file for writing
    fwrite("Moved %.0lfcm in a direction of %.2lf degrees\n", distance_moved, angle_turned, fp);      // Add contents to the file
    fclose(fp);
}


void initializePID(double target, double minOutput, double maxOutput, double initial_inputValue, double Ki, double Kd, double Kp) {
    targetValue = target;
    inputValue = initial_inputValue;
    old_inputValue = inputValue;
    min_outputValue = minOutput;
    max_outputValue = maxOutput;
    ki = Ki;
    kd = Kd;
    kp = Kp;
    timeInterval = 1.0;
}



int max_err = 20;
void getSensorLeftResponse() {
    int dacVal = 0;
    irLeft = 0;
    for(dacVal = 0; dacVal < 160; dacVal += 8) {
        dac_ctr(27, 0, dacVal);
        freqout(11, 1, 38000);
        irLeft += input(10);
    }
    if(irLeft > max_err) max_err = irLeft;
    irLeft = max_err-irLeft;
    //printf("Retrieved left_value of %d\n", irLeft);

}


void getSensorRightResponse() {
    int dacVal = 0;
    irRight = 0;
    for(dacVal = 0; dacVal < 160; dacVal += 8) {
        dac_ctr(26, 1, dacVal);
        freqout(1, 1, 38000);
        irRight += input(2);
    }
    if(irLeft > max_err) max_err = irLeft;
    irRight = max_err - irRight;
    //printf("Retrieved right_value of %d\n", irRight);
}



int printing = 0;
void proportionalIntegralDifferential() {
    clock_t currentTime = clock();
    double timeChange = currentTime - lastTime;

    if (timeChange >= timeInterval) {
        double errorValue = targetValue - inputValue;

        // Set/Update all the components for the calculation
        double proportionalComponent = errorValue * kp;
        integralSum += ki * errorValue;

        if (integralSum > max_outputValue) integralSum = max_outputValue;
        if (integralSum < min_outputValue) integralSum = min_outputValue;
        double derivativeComponent = (inputValue - old_inputValue);

        // Update values to be used in next iteration
        old_inputValue = inputValue;
        lastTime = currentTime;
        output = proportionalComponent + integralSum - kd * derivativeComponent;

        if (output > max_outputValue) output = max_outputValue;
        if (output < min_outputValue) output = min_outputValue;


        if(!printing) {
            printf("(%d(%d) - %d(%d)) = %.1lf ---> ",leftCurrentValue, irLeft, rightCurrentValue,irRight, inputValue);
            printf("P:%.1lf,%.1lf,%.1lf=>%0.3lf -> ", proportionalComponent, integralSum, (- kd * derivativeComponent), output);
            printing = 1;
        }
    }
}


void doMotion(Motion motion) {
    drive_speed(motion->right_wheel_speed, motion->left_wheel_speed);
    clock_t start = clock(), current = clock();
    pause(100);
    print(" - Doing %d, %d, %d\n", motion->time_taken, motion->right_wheel_speed, motion->left_wheel_speed);
    print("Time start: %d\n", start);
    while(current - start < motion->time_taken) current = clock(), print("Time current: %d\n", current);
    drive_speed(0, 0);
}

void followMotions(Motion motion) {
    while(motion) {
        print("Doing motion.");
        doMotion(motion);
        motion = motion->prevMotion;
    }
    return;
}


void runPID() {

    //while (1) {
        getSensorLeftResponse();
        getSensorRightResponse();
        /*if (!irLeft && irRight) leftCurrentValue += 1.5;
        else leftCurrentValue -= SENSITIVITY/10.0;
        if (!irRight && irLeft) rightCurrentValue += 1.5;
        else rightCurrentValue -= SENSITIVITY/10.0;*/

        leftCurrentValue = irLeft;
        rightCurrentValue = irRight;

        //if(leftCurrentValue > SENSITIVITY) leftCurrentValue = SENSITIVITY;
        //if(rightCurrentValue > SENSITIVITY) rightCurrentValue = SENSITIVITY;

        //if(leftCurrentValue < 0) leftCurrentValue = 0;
        //if(rightCurrentValue < 0) rightCurrentValue = 0;

        inputValue =(double) (rightCurrentValue-leftCurrentValue);
        //printf("Left,right: (%d, %d)\nPIDINPUT:%lf\n", leftCurrentValue, rightCurrentValue, inputValue);
        proportionalIntegralDifferential();
        //printf("PIDOUTPUT:%lf\n", output);
    //}
}

void checkDistance() {
    while(1) {
        if (ping_cm(8) < 7 && running == 1) {running = 0;}
        if(!running)  drive_speed(0, 0);
    }
}

void wallFollow() {
    initializePID(0, -30.0, 30.0, 0.0, 0.01, 2.1, 0.8);
    //initializePID(0, 0, 100, 50, 10, 10, 10, 0);
    //int *PID_COG_ID = cog_run(runPID, 128);
    last_call_time = clock();
    cog_run(checkDistance, 128);
    while (running) {
        runPID();
        setRobotSpeed((int)output);
    }
    running = -1;
    drive_speed(0, 0);
    //cog_end(PID_COG_ID);
    safeZeroTurn(180.0);
    //followMotions(robotMotions);
}


void setRobotSpeed(int val) {
    static int last_left = 0, last_right = 0;
    static int last_call_val = 0;
    if(last_call_val == val) return;
    else last_call_val = val;
    int value = (val+50) > 100 ? 100:((val+50) < 0 ? 0 : val+50);

    int left;
    int right;


    left = value;

    right = 100 - value;

    //printf("Intermediate left, right (%d, %d):", left, right);
    double ratio = 128.0 / 100;

    left = (int)(((double) left )* ratio);

    right = (int)(((double) right) * ratio);
    //printf("Drive Value: %d\n ", val);
    if(printing) {
        printf("%d:(%d,%d)\n", value,left,right);
        printing = 0;
    }
    //printf("drive_speed(%d, %d)\n", left, right);


    Motion motion = malloc(sizeof(motion_value));


    motion->left_wheel_speed = last_left;
    motion->right_wheel_speed = last_right;
    motion->time_taken = clock() - last_call_time;
    motion->prevMotion = robotMotions;

    last_left = left;
    last_right =right;

    robotMotions = motion;
    last_call_time = clock();


    drive_speed(left, right);
}
int first_left_dist;
int first_right_dist;
void doCalculations() {
    double dLeft = last_left_call - first_left_dist;
    double dRight = last_right_call - first_right_dist;
    dLeft *= 3.25;
    dRight *= 3.25;
    double dWheels =  dLeft - dRight;
    double theta = dWheels/105.8;
    double angle = (theta/(2*PI))*360;
    double r = (dLeft < dRight ? dLeft : dRight)/theta;
    distance_moved = r;
    angle_turned = angle;
}


int main() // Main function
{
    drive_getTicks(&last_left_call, &last_right_call);
    first_left_dist = last_left_call;
    first_right_dist = last_right_call;

    //while(1)drive_speed(20,20);
    wallFollow();
   /* cog_run(getSensorValues, 128);
    print("Running main");


    while (1) {

        //getSensorValues();
        //printf("The value of drive_speed is %d\n", driveValue);

        //printf("Retrieved right_value of %d\n", irRight);
        //printf("Retrieved left_value of %d\n", irLeft);
        updateDrive();

    }*/
}
