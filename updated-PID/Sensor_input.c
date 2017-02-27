#include "Sensor_input.h"
#include "Common_Drive_data.h"

int getIrLeft() {
    if(r_irLeft) return -1;
    return irLeft;
}

int getIrRight() {
    if(r_irRight) return -1;
    return irRight;
}

static int sweepSensor(int emmittor_pin, int sensor_pin, int power_pin) {
    int res = 0;
    int i  = 0;
    for(i = 0; i < SENSORMAXSENSITIVITY; i += SENSORINCREMENT) {
        dac_ctr(power_pin, 0, i);
        freqout(emmittor_pin, SAMPLETIME, IRFREQ);
        res += input(sensor_pin);
    }

    return res;
}

void getSensorRight() {
    static int calls = 0;
    static int prev_counts = 0;

    calls++;

    r_irRight = 1;
    irRight = 0;
    irRight = sweepSensor(1,2,26);


    prev_counts += irRight;
    if(calls < SMOOTHING_FACTOR) {
        irRight = (int)(float)prev_counts/(float)calls;
    } else {
        calls = 0;
        prev_counts = 0;
    }

    r_irRight = 0;
}

void getSensorLeft() {
    static int calls = 0;
    static int prev_counts = 0;

    calls++;

    r_irLeft = 1;
    irLeft = 0;
    irLeft = sweepSensor(11,10,27);


    prev_counts += irLeft;
    if(calls < SMOOTHING_FACTOR) {
        irLeft = (int)(float)prev_counts/(float)calls;
    } else {
        calls = 0;
        prev_counts = 0;
    }

    r_irLeft = 0;
}

/**
 * @brief Gets the sensor values using the functions configured
 * in sensor input - redefinng SMOOTHING_FACTOR, SENSORMAXSENSITIVITY,
 * SENSORINCREMENT, IRFREQ, SAMPLETIME will change the output of this.
 *
 */

wheel_values getSensorValues() {
    wheel_values result;
    getSensorRight();
    getSensorLeft();
    result.left = getIrLeft();
    result.right = getIrRight();
    return result;
}


int getWallDistance() {
    int ping_dist;
    if(ESTIMATED_PING) {
        int __ping_dist = ping(8);
        ping_dist = (int) roundf(((float)0.0172f*__ping_dist - (float)0.706f));
    } else {
        ping_dist = ping_cm(8);
    }
    return ping_dist;
}
