#include "abdrive.h"
#include <stdio.h>
#include "simpletools.h"

//All conversion between degrees to ticks is handled
//by the function, allowing the user to maintain a 
//constant set of units.
void zeroRadTurn(float angle){
  //Calculate the angle to move.
  float result = (((angle/360)*PI*2)*(105.8/2))/3.25;
  
  //Move the angle.
  drive_goto((int)result, -1 * (int)result);
  
  return;
}


int main() 
{
  while(1){
  zeroRadTurn(360);
 }
}
