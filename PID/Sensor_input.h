#ifndef ROBOTSIMULATORNATIVECLIENT_SENSOR_INPUT_H
#define ROBOTSIMULATORNATIVECLIENT_SENSOR_INPUT_H

#include "simpletools.h"
#include "abdrive.h"
#include <ping.h>

#include "Common_Drive_data.h"

#define SENSORMAXSENSITIVITY 256
#define SENSORINCREMENT 8
#define IRFREQ 38000
#define SAMPLETIME 1
#define SMOOTHING_FACTOR 1

#ifndef ESTIMATED_PING
#define ESTIMATED_PING 1
#endif


volatile int irLeft;
volatile int irRight;
volatile static int r_irLeft = 0, r_irRight = 0;

int getIrLeft();
int getIrRight();
static int sweepSensor(int emmittor_pin, int sensor_pin, int power_pin);
void getSensorRight();
void getSensorLeft();
wheel_values getSensorValues();
int getWallDistance();


#endif //ROBOTSIMULATORNATIVECLIENT_SENSOR_INPUT_H
