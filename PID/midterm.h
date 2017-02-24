//
// Created by gopia on 23/02/2017.
//

#ifndef ROBOTSIMULATORNATIVECLIENT_MIDTERM_H
#define ROBOTSIMULATORNATIVECLIENT_MIDTERM_H

#include "PID.h"

typedef struct {
    wheel_values values;
    clock_t time;
} motion;

typedef struct _motionStackPlate {
    motion motion;
    struct _motionStackPlate *prev;
} *motionStackPlate,amotionStackPlate;

typedef struct _motionStack {
    motionStackPlate head;
} *motionStack, amotionStack;

int wall_detected = 0;

#endif //ROBOTSIMULATORNATIVECLIENT_MIDTERM_H
