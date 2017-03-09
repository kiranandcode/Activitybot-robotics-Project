#include "sensor_input.h"

#ifdef AUTOTUNE_IR_SENSITIVITY
int sensor_sensitivity = 20;
#endif


void autotune() {
#ifdef AUTOTUNE_IR_SENSITIVITY
while(getIrLeft() == 0 && getIrRight() == 0) {
#ifdef DEBUG
    print("Sensitivity (%d) not high enough. Increasing by 20.\n", sensor_sensitivity);
#endif
    sensor_sensitivity+=20;
}
#endif
}

static int sweepSensor(int emmittor_pin, int sensor_pin, int power_pin) {
    int res = 0;
    int i  = 0;
    for(i = 0; i < SENSOR_MAX_SENSITIVITY; i += SENSOR_INCREMENT) {
        dac_ctr(power_pin, 0, i);
        freqout(emmittor_pin, SAMPLETIME, IRFREQ);
        res += input(sensor_pin);
    }

    return res;
}


void getSensorRight() {
    irRight = 0;
    irRight = sweepSensor(11, 10, 27);
}

void getSensorLeft() {
    irLeft = 0;
    irLeft = sweepSensor(1, 2, 26);
}

int getIrLeft() {
    getSensorLeft();
    return irLeft;
}

int getIrRight() {
    getSensorRight();
    return irRight;
}

int checkConsistent(int *values) {
    int i;
    int val = values[0];
    for(i = 1; i<SENSOR_SAMPLE_SIZE; i++) {
        if(abs(values[i]-val) > SENSOR_CONSISTENCY_BLUR) return 0;
    }
    return 1;
}

int getConsistentResults(int (*function)()) {
    int values[SENSOR_SAMPLE_SIZE];
    int i;
    for(i = 0; i<SENSOR_SAMPLE_SIZE; i++) values[i] = 0;
    do {
        for(i = 0; i<SENSOR_SAMPLE_SIZE; i++) {
            values[i] = function();
#ifdef DEBUG
#ifndef SENSOR_DEBUG_OFF
    printi("values[%d]: %d\n", i, values[i]);
#endif
#endif
        }
    } while(!checkConsistent(values));
    return values[0];
}


double getAvgResults(int (*function)()) {
    double sum = 0;
    int i;
    for(i = 0; i<SENSOR_SAMPLE_SIZE; i++) sum += function();
    double avg = sum / SENSOR_SAMPLE_SIZE;
    return avg;
}


tuplePtr getSensorValues() {
    tuplePtr result = malloc(sizeof(tuple));
    getSensorRight();
    getSensorLeft();
    result->left = getConsistentResults(getIrLeft);
    result->right = getConsistentResults(getIrRight);
    return result;
}

duplePtr getAvgSensorValues() {
    duplePtr result = malloc(sizeof(duple));
    getSensorRight();
    getSensorLeft();
    result->left = getAvgResults(getIrLeft);
    result->right = getAvgResults(getIrRight);
    return result;
}


int getWallDistance() {
    int ping_dist;
    if(ESTIMATED_PING) {
        int __ping_dist = ping(8);
        ping_dist = (int)(((float)0.0172f*__ping_dist - (float)0.706f));
    } else {
        ping_dist = ping_cm(8);
    }
    return ping_dist;
}