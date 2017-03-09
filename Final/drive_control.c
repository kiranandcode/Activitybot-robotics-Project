//
// Created by gopia on 08/03/2017.
//

#include "drive_control.h"


void drive_goForward(int left_dist, int right_dist){
#ifdef DEBUG
#ifdef DRIVE_CALLS
    print("drive_goForward(%d, %d)\n", left_dist, right_dist);
#endif
#endif
    double theta = ((double) ((left_dist - right_dist) * 3.25) / 105.8);
    double conv_left_dist = (double) left_dist * 3.25;
    double conv_right_dist = (double) right_dist * 3.25;
    if (theta != 0) {
        double r_inner = (double) conv_right_dist / theta;
        double r_outer = (double) conv_left_dist / theta;
        double r_middle = (r_inner + r_outer) / 2;
        double dy = (r_middle * sin(theta + angle)) - (r_middle * sin(angle));
        double dx = (r_middle * cos(angle)) - (r_middle * cos(angle + theta));

        x += dx;
        y += dy;
        angle += theta;
        if(angle > 2*PI) angle -= 2*PI;
    } else {
        double dx = ((double) (conv_left_dist + conv_right_dist) / 2) * sin(angle);
        double dy = ((double) (conv_left_dist + conv_right_dist) / 2) * cos(angle);

        x += dx;
        y += dy;
    }
    drive_goto(left_dist, right_dist);
}

void driveTo(int px, int py) {
#ifdef DEBUG
#ifdef DRIVE_CALLS
    print("driveTo(%d, %d)\n", px, py);
#endif
#endif


#ifdef DEBUG
#ifndef POSITION_DEBUG_OFF
    print("xy: (%lf, %lf)\n", x, y);
#endif
#endif
    double mmposx = px*3.25;
    double mmposy = py*3.25;
    double dx = mmposx - x;
    double dy = mmposy - y;
    if((dx == dy) && (dy == 0)) return;
    double bearing = atan2(dx,dy);
    double angle_dif = bearing - angle;
    //printf("In order to move from %lf %lf to %d %d");
    turn(angle_dif);
    double radius = sqrt(dx*dx + dy*dy);
    drive_goForward((int)radius/3.25, (int)radius/3.25);
}


void turn(double theta) {
    static double rollover = 0;
#ifdef DEBUG
#ifdef DRIVE_CALLS
    print("turn(%lf)\n", theta);
#endif
#endif
    theta+= rollover;
    if(theta > PI) theta = theta-2*PI;
    if(theta < -PI) theta = 2*PI + theta;
    double ticks = (RADIUS*theta)/MM_P_TICK;
    drive_goto((int)(ticks), (int)(-1 * ticks));
    double turned = (((int)ticks)*MM_P_TICK)/RADIUS;
    angle += turned;
    rollover = (theta-turned);
    if(angle > 2*PI) angle -= 2*PI;
}