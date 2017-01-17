#include "abdrive.h"
#include <stdio.h>
#include "simpletools.h"
                    // Include simple tools

void zeroRadTurn(float angle){ 
  float result = (((angle/360)*PI*2)*(105.8/2))/3.25;
  drive_goto((int)result, -1 * (int)result);
  return;
}






int main()                                    // Main function
{
  while(1){
  zeroRadTurn(360);
 }                                            // Display test message
}
