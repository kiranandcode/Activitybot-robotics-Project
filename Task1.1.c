#include "abdrive.h"
#include <stdio.h>
#include "simpletools.h"

float safeZeroTurn(float desired_angle) {
  float current_angle = 0;
  float delta_angle = desired_angle;
  while(current_angle < desired_angle) {
    int start_distance[2];
    int end_distance[2]; 
    drive_getTicks(start_distance, start_distance+1);
    float result = (((delta_angle/360)*PI*2)*(105.8/2))/3.25;
    drive_goto((int)result, -1 * (int)result);
    drive_getTicks(end_distance, end_distance+1);
    float avg_dif = (abs(end_distance[0]-start_distance[0])+abs(end_distance[1]-start_distance[1]))/2;
    float angle_dif = avg_dif * (3.25*360)/(52.9*2*PI);
  current_angle += angle_dif;
  delta_angle = desired_angle - current_angle;
    if(delta_angle < 3) break; 
  }
  return delta_angle;
}


void safeDrawLine(float desired_dist) {
   float current_dist = 0;
   float delta_dist;
   while(current_dist < desired_dist) {
    int start_distance[2];
    int end_distance[2]; 
    int tick_to_move = mmToTicks(delta_dist);
    drive_getTicks(start_distance, start_distance+1);
    drive_goto(tick_to_move, tick_to_move);
    drive_getTicks(end_distance, end_distance+1);
    float avg_dif = (abs(end_distance[0]-start_distance[0])+abs(end_distance[1]-start_distance[1]))/2;
    current_dist += avg_dif*3.25;
    delta_dist = desired_dist - current_dist;
    if(delta_dist<3) break;
  }  
}    

void safeDrawSquare(float side_length) {
 int i;
 float angleLoss = 0.0F;
 for(i = 0; i<4; i++) {
  safeDrawLine(side_length);
  angleLoss = safeZeroTurn(90+angleLoss); 
 }    
  return;
}  


int main()
{
  print("Drawing Square\n");
  safeDrawSquare(1000);
  print("Square Drawn\n");
}

