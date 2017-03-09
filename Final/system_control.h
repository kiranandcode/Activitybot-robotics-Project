//
// Created by gopia on 08/03/2017.
//

#ifndef ROBOTSIMULATORNATIVECLIENT_SYSTEM_CONTROL_H
#define ROBOTSIMULATORNATIVECLIENT_SYSTEM_CONTROL_H

#include "system_settings.h"
#include "sensor_input.h"
#include "drive_control.h"

#define INITIAL_SIZE 10

extern double dleft, dright;
extern int size;

void center();
int driveforconsistent();
void readjust();
tuplePtr travelToDeadEnd();
void followPath(tuplePtr path);

#endif //ROBOTSIMULATORNATIVECLIENT_SYSTEM_CONTROL_H
