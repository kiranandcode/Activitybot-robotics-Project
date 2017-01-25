#include "abdrive.h"
#include <stdio.h>
#include "simpletools.h"

int mmToTicks(float mm) {
 return mm/3.25; 
}

//For the project, we decided to use a standard set of
//units throughout the program to prevent errors later
//down the line. These units were chosen to be mm and
//degrees.


void moveForward(int length) {
  int ticks = mmToTicks(length);
  drive_goto(ticks, ticks);
  return;
} 





int main()   
{
  moveForward(500);
}
