#include "system_control.h"
double dleft, dright;

int size = INITIAL_SIZE;
// 0 for done
// 1 for readjustment needed
int driveforconsistent() {
#ifdef AVG_IR_SENSORS
    duplePtr sensor_values = NULL;
#else
    tuplePtr sensor_values = NULL;
#endif

    double last_left = -1, last_right = -1, first_run = 1;
    do {
        if(sensor_values != NULL) free(sensor_values);
#ifdef AVG_IR_SENSORS
        sensor_values = getAvgSensorValues();
#else
        sensor_values = getSensorValues();
#endif

        printf("Robot: %lf %lf\n", sensor_values->left, sensor_values->right);

#ifdef AVG_PING_SENSORS
        double forward_dist = 10*getAvgResults(getWallDistance)/3.25;
#else
        double forward_dist = 10*getConsistentResults(getWallDistance)/3.25;
#endif

        printf("%lf\n", forward_dist);
        // We have reached the end - obv. stop.
        if(forward_dist < MIN_DISTANCE) {
            if(sensor_values != NULL) free(sensor_values);
            return 0;
        }else {
#ifdef AVG_IR_SENSORS
          if(fabs(sensor_values->right - sensor_values->left) < AVG_SENSOR_DEADZONE){

#else
            //if(sensor_values->right == sensor_values->left){
#endif
              if(first_run) {
                  last_left = sensor_values->left;
                  last_right = sensor_values->right;
                  first_run = 0;
              } else {
                  dleft = last_left - sensor_values->left;
                  dright = last_right - sensor_values->right;
                  if(fabs(dleft-dright)> DRIVE_WALL_SLOPE_SENSITIVITY)
                  {
                      if(sensor_values != NULL) free(sensor_values);
                      return 1;
                  }
                  last_left = sensor_values->left;
                  last_right = sensor_values->right;
              }
              drive_goForward(DRIVE_DIST, DRIVE_DIST);
          } else if(fabs(sensor_values->right - sensor_values->left) < DRIVE_ERROR_THRESHHOLD){
               if(!first_run) {
                  dleft = last_left - sensor_values->left;
                  dright = last_right - sensor_values->right;
                  if(abs(dleft-dright)> DRIVE_WALL_SLOPE_SENSITIVITY) {
                      if(sensor_values != NULL) free(sensor_values);
                      return 1;
                  }
              }
              center();
              drive_goForward(DRIVE_DIST, DRIVE_DIST);
          } else {
              if(sensor_values != NULL) free(sensor_values);
              return 1;
          }


        }
    } while(1);
}

void center() {
    turn(PI/2);
    double right_dist = 10*getAvgResults(getWallDistance)/3.25;
    turn(-PI);
    double left_dist = 10*getAvgResults(getWallDistance)/3.25;

    double difference = ((double)abs(left_dist-right_dist));

    if(left_dist > right_dist) {
        drive_goForward(difference/2,difference/2);
        turn(PI/2);
    } else {
        turn(PI);
        drive_goForward(difference/2,difference/2);
        turn(-PI/2);
    }
}

void readjust() {
    turn(PI/2);
    double old_right_dist = 10*getAvgResults(getWallDistance)/3.25;
    turn(-PI);
    double old_left_dist = 10*getAvgResults(getWallDistance)/3.25;
    turn(PI/2);
    drive_goForward(CHECK_DIST, CHECK_DIST);

    turn(PI/2);
    double new_right_dist = 10*getAvgResults(getWallDistance)/3.25;
    turn(-PI);
    double new_left_dist = 10*getAvgResults(getWallDistance)/3.25;
    turn(PI/2);

    double d_left = new_left_dist-old_left_dist;
    double d_right = new_right_dist - old_right_dist;

    double distance_travelled = CHECK_DIST * 3.25;
    double left_angle = atan2(d_left, distance_travelled);
    double right_angle = atan2(d_right, distance_travelled);
    printf("Moved %lf, walls changed %lf %lf, angle_left %lf, angle_right %lf\n", distance_travelled, d_left, d_right, left_angle, right_angle);
    double avg_angle = (left_angle-right_angle)/2;

    double angle_diff = -1* ANGLE_SF *avg_angle;
    printf("Turning %lf\n", angle_diff);
    turn(angle_diff);
    drive_goForward(DRIVE_DIST, DRIVE_DIST);
}

tuplePtr travelToDeadEnd() {
    tuplePtr entries = malloc(sizeof(tuple) * size);
    entries->right = 0;
    entries->left = 0;
    int status;
    int i = 1;
    do {
        status = driveforconsistent();
        printf("status:%d %d\n", status, i);
        if(status) readjust();
        (entries+i)->x = (int)x/3.25;
        (entries+i)->y = (int)y/3.25;
        i++;
        if(i >= size) {size += INITIAL_SIZE; entries = realloc(entries, size * sizeof(tuple));}
    } while(status);
    entries = realloc(entries, sizeof(tuple)*(i));
    size = i;
    printf("Done\n");
    return entries;
}


void followPath(tuplePtr path) {
    tuplePtr start = path;
    int i;
    for(i = size-1; i>=0; i--) {
        printf("CurrentPos{%lf %lf) -> %d %d\n", x, y, start[i].left, start[i].right);
        driveTo(start[i].left, start[i].right);
    }
    free(start);
}