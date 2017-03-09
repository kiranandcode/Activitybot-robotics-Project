//
// Created by gopia on 07/03/2017.
//

#ifndef ROBOTSIMULATORNATIVECLIENT_SENSOR_INPUT_H
#define ROBOTSIMULATORNATIVECLIENT_SENSOR_INPUT_H
#include "abdrive.h"
#include "ping.h"
#include "simpletools.h"
#include "system_settings.h"

#ifdef AUTOTUNE_IR_SENSITIVITY
#define SENSOR_MAX_SENSITIVITY sensor_sensitivity
extern int sensor_sensitivity;
#else
#ifndef SENSOR_MAX_SENSITIVITY
#define SENSOR_MAX_SENSITIVITY 20
#endif
#endif
#ifndef SENSOR_INCREMENT
#define SENSOR_INCREMENT 1
#endif
#ifndef ESTIMATED_PING
#define ESTIMATED_PING 1
#endif
#ifndef SENSOR_SAMPLE_SIZE
#define SENSOR_SAMPLE_SIZE 20
#endif
#ifndef SENSOR_CONSISTENCY_BLUR
#define SENSOR_CONSISTENCY_BLUR 0
#endif







#define IRFREQ 38000
#define SAMPLETIME 1

volatile int irLeft;
volatile int irRight;

void autotune();
int getIrLeft();
int getIrRight();
static int sweepSensor(int emmittor_pin, int sensor_pin, int power_pin);
void getSensorRight();
void getSensorLeft();
int getConsistentResults(int (*function)());
double getAvgResults(int (*function)());
tuplePtr getSensorValues();
duplePtr getAvgSensorValues();
int getWallDistance();

#endif //ROBOTSIMULATORNATIVECLIENT_SENSOR_INPUT_H
