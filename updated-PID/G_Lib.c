//
// Created by gopia on 26/02/2017.
//

#include "G_Lib.h"
void safeZeroTurn(float angle_degrees) {

    //initialize angle_to_turn to 0 once.
    static float angle_to_turn = 0;

    int start_distance[2];
    int end_distance[2];

    //convert to radians - loss-less
    float desired_angle_radians = angle_degrees * 2.0 * PI / 360.0;

    //add this angle to angle to turn;
    angle_to_turn += desired_angle_radians;

    //calculate ticks to turn - lossy.
    int ticks_to_turn = RADIUS * angle_to_turn / TICKS_P_MM;

    //calculate exact loss in angle from conversion.
    float conversion_angle_loss = angle_to_turn - (ticks_to_turn*TICKS_P_MM/RADIUS);

    //actually move through the turn.
    drive_getTicks(start_distance, start_distance+1);
    drive_goto(ticks_to_turn, -1 * ticks_to_turn);
    drive_getTicks(end_distance, end_distance+1);

    //calculate the ticks actually turned.
    int ticks_turned = end_distance[0] - start_distance[0]; // + (-1) * (end_distance[1] - start_distance[1])); /2

    //calculate the loss in ticks.
    int delta_ticks = ticks_to_turn - ticks_turned;
    printf("Ticks to turn, Ticks turned: %d,%d\nLoss in ticks: %d\n",ticks_to_turn, ticks_turned, delta_ticks);

    //calculate loss in angle due to loss in ticks.
    float movement_angle_loss = delta_ticks * TICKS_P_MM / RADIUS;

    //store this loss for the next iteration.
    angle_to_turn = conversion_angle_loss + movement_angle_loss;
    printf("Angle-loss: %f\n", angle_to_turn);
    return;
}