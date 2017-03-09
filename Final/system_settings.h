//
// Created by gopia on 07/03/2017.
//

#ifndef ROBOTSIMULATORNATIVECLIENT_SYSTEM_SETTINGS_H
#define ROBOTSIMULATORNATIVECLIENT_SYSTEM_SETTINGS_H

//#define DEBUG
//#define SENSOR_DEBUG_OFF
//#define POSITION_DEBUG_OFF

//#define DRIVE_CALLS

//#define AUTOTUNE_IR_SENSITIVITY
#define SENSOR_MAX_SENSITIVITY 160
#define SENSOR_INCREMENT 8
//#define ESTIMATED_PING 0
#define SENSOR_SAMPLE_SIZE 5
//#define SENSOR_CONSISTENCY_BLUR 0
#define AVG_IR_SENSORS
#define AVG_PING_SENSORS
#define AVG_SENSOR_DEADZONE 1.5
#define ANGLE_SF 5




#define MIN_DISTANCE 50
//#define DRIVE_DIST 15
//#define RESOLUTION 20
#define CHECK_DIST 15
//#define DRIVE_ERROR_THRESHHOLD 3
//#define DRIVE_WALL_SLOPE_SENSITIVITY 0





typedef struct tuple {
    union {
        int left;
        int x;
    };
    union {
        int right;
        int y;
    };
} tuple, *tuplePtr;

typedef struct duple {
    union {
        double left;
        double x;
    };
    union {
        double right;
        double y;
    };
} duple, *duplePtr;

extern double x,y;
extern double angle;

#endif //ROBOTSIMULATORNATIVECLIENT_SYSTEM_SETTINGS_H
