#include "abdrive.h"
#include <stdio.h>
#include "simpletools.h"
                    // Include simple tools

int mmToTicks(float mm) {
 return mm/3.25; 
}

void moveForward(int length) {
  int ticks = mmToTicks(length);
  drive_goto(ticks, ticks);
  return;
} 





int main()                                    // Main function
{

  moveForward(500);
}
