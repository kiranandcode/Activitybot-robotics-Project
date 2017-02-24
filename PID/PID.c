
#include "PID.h"
void _initialize(psensor_function lrvals, sensor_function front, double KI, double KD, double KP) {
    getLRValues = lrvals;
    getFront = front;
    ki = KI;
    kd = KD;
    kp = KP;
    target = TARGET;
    //pid_setup = 1;
}

void setInput(wheel_values LRValues) {
    Input = LRValues.left - LRValues.right;
    //valid_input = 1;
}

void calculateOutput() {
    static double last_error = 0;
    static double error_sum = 0;
    int error = target - Input;

    double proportionalComponent = kp * (double) (error);
    double derivativeComponent = kd * (double) (error-last_error)/(double)PID_INTERVAL;
    double integralComponent = (error_sum += ki * (double)error);

    Output = proportionalComponent + integralComponent - derivativeComponent;
    //valid_input = 0;
}

void executePID() {
    static clock_t last_call = 0;
    clock_t current_call_time = clock();

    //Initializing the last_call
    if(last_call == 0) last_call = current_call_time;


    if(last_call - current_call_time < PID_INTERVAL) return;
    last_call = current_call_time;

    wheel_values LeftRight = getLRValues();
    setInput(LeftRight);
    calculateOutput();
}

wheel_values getOutput() {
    int value = (Output+50) > 100 ? 100:((Output+50) < 0 ? 0 : Output+50);

    wheel_values result;

    result.left = value;

    result.right = 100 - value;

    double ratio = MAX_OUTPUT_SPEED / (double)100;

    result.left = (int)(((double) result.left )* ratio);

    result.right = (int)(((double) result.right) * ratio);
    return result;
}
