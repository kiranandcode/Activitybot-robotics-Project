#include "abdrive.h"
#include <stdio.h>
#include "simpletools.h"
                    // Include simple tools


float tickToMM(int ticks) {
  drive_open();
 return 3.25*ticks;
 drive_close(); 
}  

int mmToTicks(float mm) {
 return mm/3.25; 
}

float radToDeg(float rad) {
 return rad/(2*PI)*360; 
}

float degToRad(float deg) {
 return deg/360*(2*PI); 
}  

void moveForward(int length) {
  int ticks = mmToTicks(length);
  drive_goto(ticks, ticks);
  return;
} 

void zeroRadTurn(float angle){ 
  float result = (((angle/360)*PI*2)*(105.8/2))/3.25;
  drive_goto((int)result, -1 * (int)result);
  return;
}






int main()                                    // Main function
{
  while(1){
 moveForward(50); 
 print("hello");
 }                                            // Display test message
}
