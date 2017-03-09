//
// Created by gopia on 08/03/2017.
//

#ifndef ROBOTSIMULATORNATIVECLIENT_DRIVE_CONTROL_H
#define ROBOTSIMULATORNATIVECLIENT_DRIVE_CONTROL_H

#include "system_settings.h"
#include "abdrive.h"
#include <math.h>

#define MM_P_TICK 3.25
#define RADIUS 52.9

#ifndef MIN_DISTANCE
#define MIN_DISTANCE 15
#endif
#ifndef DRIVE_DIST
#define DRIVE_DIST 15
#endif
#ifndef CHECK_DIST
#define CHECK_DIST 10
#endif
#ifndef RESOLUTION
#define RESOLUTION 20
#endif
#ifndef DRIVE_ERROR_THRESHHOLD
#define DRIVE_ERROR_THRESHHOLD 3
#endif
#ifndef DRIVE_WALL_SLOPE_SENSITIVITY
#define DRIVE_WALL_SLOPE_SENSITIVITY 0.5
#endif




extern double x,y;
extern double angle;

void drive_goForward(int left_dist, int right_dist);
void driveTo(int px, int py);
void turn(double theta);

#endif //ROBOTSIMULATORNATIVECLIENT_DRIVE_CONTROL_H
