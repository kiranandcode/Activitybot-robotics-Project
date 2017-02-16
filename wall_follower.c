#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "simpletools.h" // Include simple tools
#include "abdrive.h"
#include "simpletext.h"
#include "cog.h"


#define  SENSITIVITY 20
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
int controlDirection = 0;

int leftCurrentValue = 0;
int rightCurrentValue = 0;

void initializePID(double target, double minOutput, double maxOutput, double initial_inputValue, double Ki, double Kd, double Kp, int controlDir) {
    controlDirection = controlDir;

    targetValue = target;
    inputValue = initial_inputValue;
    old_inputValue = inputValue;
    min_outputValue = minOutput;
    max_outputValue = maxOutput;
    ki = Ki;
    kd = Kd;
    kp = Kp;
    pid_activated = 1;
    pid_initialized = 1;
}
int printing = 0;
void proportionalIntegralDifferential() {
    clock_t currentTime = clock();
    double timeChange = currentTime - lastTime;
    if (!pid_initialized || !pid_activated) return;
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
        //printf("pC, iSum, dC: %lf, %lf, %lf\n", proportionalComponent, integralSum, derivativeComponent);
        //printf("Recorded ErrorVal: %lf, output: %lf\n", errorValue, output);

        if (output > max_outputValue) output = max_outputValue;
        if (output < min_outputValue) output = min_outputValue;


        if(!printing) {
            printf("(%d(%d) - %d(%d)) = %.1lf ---> ",leftCurrentValue, irLeft, rightCurrentValue,irRight, inputValue);
            printf("P:%.1lf,%.1lf,%.1lf=>%0.3lf -> ", proportionalComponent, integralSum, (- kd * derivativeComponent), output);
            printing = 1;
        }
    }
}

void setConstants(double Ki, double Kp, double Kd) {
    if (Kp < 0 || Ki < 0 || Kd < 0) return;
    double timeIntervalSec = timeInterval / 1000;
    kp = Kp;
    ki = Ki * timeIntervalSec;
    kd = Kd / timeIntervalSec;

    if (controlDirection == 1) {
        kp = 0 - kp;
        ki = 0 - ki;
        kd = 0 - kd;
    }
}

void setControlDirection(int direction) {
    if (direction != 0 && direction != 1) return;
    controlDirection = direction;
}

void setTimeInterval(double newTime) {
    if (newTime > 0) {
        double ratio = newTime / timeInterval;
        timeInterval = newTime;
        ki *= ratio;
        kd /= ratio;
    }
}

void setOutputLimits(double min, double max) {
    if (min > max_outputValue) return;
    min_outputValue = min;
    max_outputValue = max;

    if (output > max_outputValue) output = max_outputValue;
    if (output < min_outputValue) output = min_outputValue;

    if (integralSum > max_outputValue) integralSum = max_outputValue;
    if (integralSum < min_outputValue) integralSum = min_outputValue;
}

void internalReInitialize() {
    old_inputValue = inputValue;
    integralSum = output;
    if (integralSum > max_outputValue) integralSum = max_outputValue;
    if (integralSum < min_outputValue) integralSum = min_outputValue;
}

void activate() {
    if (pid_activated) pid_activated = 0;
    else pid_activated = 1;
}
void runPID() {

    //while (1) {
        getSensorLeftResponse();
        getSensorRightResponse();
        if (!irLeft) leftCurrentValue++;
        else leftCurrentValue -= SENSITIVITY/2;
        if (!irRight) rightCurrentValue++;
        else rightCurrentValue -= SENSITIVITY/2;

        if(leftCurrentValue > SENSITIVITY) leftCurrentValue = SENSITIVITY;
        if(rightCurrentValue > SENSITIVITY) rightCurrentValue = SENSITIVITY;

        if(leftCurrentValue < 0) leftCurrentValue = 0;
        if(rightCurrentValue < 0) rightCurrentValue = 0;

        inputValue =(double) (rightCurrentValue-leftCurrentValue);
        //printf("Left,right: (%d, %d)\nPIDINPUT:%lf\n", leftCurrentValue, rightCurrentValue, inputValue);
        proportionalIntegralDifferential();
        //printf("PIDOUTPUT:%lf\n", output);
    //}
}



void wallFollow() {
    initializePID(SENSITIVITY, -50.0, 50.0, 0.0, 0.0, 0.0, 2.3, 0);
    setTimeInterval(1.0);
    //initializePID(0, 0, 100, 50, 10, 10, 10, 0);
    //cog_run(, 128);
    while (1) {
        runPID();
        setRobotSpeed((int)output);
    }
}


void clampDriveValue() {

    if (driveValue > 100) driveValue = 100;

    if (driveValue < 0) driveValue = 0;

}

void setRobotSpeed(int val) {
    static int last_call_val = 0;
    //if(last_call_val == val) return;
    //else last_call_val = val;
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
    drive_speed(left, right);
}


void getSensorLeftResponse() {

    freqout(11, 1, 38000);
    irLeft = input(10);
    //printf("Retrieved left_value of %d\n", irLeft);

}


void getSensorRightResponse() {

    freqout(1, 1, 38000);
    irRight = input(2);
    //printf("Retrieved right_value of %d\n", irRight);
}


void getSensorValues() {
    while (1) {
        if (!sensor_reading_completed) {
            getSensorLeftResponse();
            getSensorRightResponse();
            sensor_reading_completed = 1;
        }
    }
}


void updateDrive() {
    if (sensor_reading_completed) {
        int old_driveValue = driveValue;
        if (!irRight) driveValue++;

        if (!irLeft) driveValue--;

        clampDriveValue();
        if (old_driveValue != driveValue) {
            setRobotSpeed(driveValue);
        }
        sensor_reading_completed = 0;
    }
}


int main() // Main function
{
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
