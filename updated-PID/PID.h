//
// Created by gopia on 24/02/2017.
//

#ifndef ROBOTSIMULATORNATIVECLIENT_PID_H
#define ROBOTSIMULATORNATIVECLIENT_PID_H

#include <time.h>
#include <stdio.h>
#include "Sensor_input.h"

#include "Common_Drive_data.h"

#define PID_INTERVAL 10
#define TARGET 0.0
#define DEFAULT_KI 0.0000000005
#define DEFAULT_KD 0.03
#define DEFAULT_KP 0.01
#define MAX_OUTPUT_SPEED 50.0
#define SENSOR_DEADZONE 1.5

#define initialize(A, B) _initialize(A,B,DEFAULT_KI, DEFAULT_KD, DEFAULT_KP)



typedef int (*sensor_function)(void);
typedef wheel_values (*psensor_function)(void);

//int pid_setup = 0;
int Input, Output;
//int valid_input = 0;
int target;

double ki, kd, kp;


psensor_function getLRValues;
sensor_function getFront;


void _initialize(psensor_function lrvals, sensor_function front, double KI, double KD, double KP);
void setInput(wheel_values LRValues);
void calculateOutput();
void executePID();
wheel_values getOutput();


#endif //ROBOTSIMULATORNATIVECLIENT_PID_H
