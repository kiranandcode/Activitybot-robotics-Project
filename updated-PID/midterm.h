//
// Created by gopia on 23/02/2017.
//

#ifndef ROBOTSIMULATORNATIVECLIENT_MIDTERM_H
#define ROBOTSIMULATORNATIVECLIENT_MIDTERM_H

#include "PID.h"
#include "simpletools.h"
#include "G_Lib.h"

typedef struct motion {
    wheel_values values;
    clock_t time;
    struct motion *prev;
} *motion, __motion__;

int wall_detected = 0;
time_t last_call_value = -1;

#endif //ROBOTSIMULATORNATIVECLIENT_MIDTERM_H
