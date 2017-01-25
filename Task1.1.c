#include "abdrive.h"
#include <stdio.h>
#include "simpletools.h"

//All safe functions use motor encoders to ensure that
//they have travelled the distance they were called with.
float safeZeroTurn(float desired_angle) {
  float current_angle = 0;
  float delta_angle = desired_angle;
  
  //Repeatedly continue turning until the current angle is
  //equal to the desired angle.
  while(current_angle < desired_angle) {
    int start_distance[2];
    int end_distance[2]; 
    
    //Calculate angle to move
    float result = (((delta_angle/360)*PI*2)*(105.8/2))/3.25;

    //Move Angle
    drive_getTicks(start_distance, start_distance+1);
    drive_goto((int)result, -1 * (int)result);
    drive_getTicks(end_distance, end_distance+1);
    
    //Calculate actual angle turned.
    float avg_dif = (abs(end_distance[0]-start_distance[0])+abs(end_distance[1]-start_distance[1]))/2;
    float angle_dif = avg_dif * (3.25*360)/(52.9*2*PI);
    
    //Update current angle and delta_angle.
    current_angle += angle_dif;
    delta_angle = desired_angle - current_angle;
    
    if(delta_angle < 3) break;
  }
  
  //Return the residue angle, unable to be turned.
  return delta_angle;
}


void safeDrawLine(float desired_dist) {
   float current_dist = 0;
   float delta_dist;
  
   while(current_dist < desired_dist) {
    int start_distance[2];
    int end_distance[2]; 

    //Calculate distance to move.
    int tick_to_move = mmToTicks(delta_dist);
     
    //move distance.
    drive_getTicks(start_distance, start_distance+1);
    drive_goto(tick_to_move, tick_to_move);
    drive_getTicks(end_distance, end_distance+1);
     
     //Calculate actual distance moved.
    float avg_dif = (abs(end_distance[0]-start_distance[0])+abs(end_distance[1]-start_distance[1]))/2;
    
    //update current distance and delta_dist.
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

