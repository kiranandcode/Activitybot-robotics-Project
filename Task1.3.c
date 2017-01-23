#include "abdrive.h"
#include <stdio.h>
#include "simpletools.h"
#define DEBUG 0

void safeZeroTurn(float desired_angle) {
  float current_angle = 0;
  static float delta_angle = 0;
	if(DEBUG) printf("\t\t%f input call - initial condition %f\n", desired_angle, delta_angle);
  delta_angle += desired_angle;
  while(abs(current_angle) < abs(desired_angle)) {
	 if(DEBUG) printf("loop - current_angle %f, %f\n", current_angle, delta_angle);
    int start_distance[2];
    int end_distance[2]; 
    drive_getTicks(start_distance, start_distance+1);
	
    float result = (((delta_angle/360)*PI*2)*(105.8/2))/3.25;
    
	drive_goto((int)result, (-1) * (int)result);
    drive_getTicks(end_distance, end_distance+1);
	float avg_dif = (abs(end_distance[0]-start_distance[0])+abs(end_distance[1]-start_distance[1]))/2;
    float angle_dif = avg_dif * (3.25*360)/(52.9*2*PI);
	if((end_distance[0]-start_distance[0]) < 0) angle_dif *= -1;
	current_angle += angle_dif;
	delta_angle = desired_angle - current_angle;
	if(DEBUG) printf("%f moved\n", delta_angle);
    if(abs(delta_angle) < 3) break; 
  }
  return;
}

int mmToTicks(float mm) {
return mm/3.25;
}

void safeDrawLine(float desired_dist) {
   float current_dist = 0;

   static float delta_dist = 0; 
   delta_dist += desired_dist;
	
	while(abs(current_dist) < abs(desired_dist)) {
	if(DEBUG) printf("%f Dist left\n", delta_dist);
    int start_distance[2];

    int end_distance[2]; 

    int tick_to_move = mmToTicks(delta_dist);

    drive_getTicks(start_distance, start_distance+1);

    drive_goto(tick_to_move, tick_to_move);

    drive_getTicks(end_distance, end_distance+1);

    float avg_dif = (abs(end_distance[0]-start_distance[0])+abs(end_distance[1]-start_distance[1]))/2;
	if((end_distance[0]-start_distance[0]) < 0) avg_dif *= -1;
    current_dist += avg_dif*3.25;

	delta_dist = desired_dist - current_dist;
		if(abs(delta_dist) < 4) break;
	}
	if(DEBUG) printf("Line Drawn\n");
}


void drawA() {

	safeDrawLine(40.0);

	safeZeroTurn(-90.0);

	safeDrawLine(120.0);

	safeZeroTurn(90.0);

	safeDrawLine(120.0);

	safeZeroTurn(90.0);

	safeDrawLine(60.0);

	safeZeroTurn(90.0);

	safeDrawLine(120.0);

	safeZeroTurn(180.0);

	safeDrawLine(120.0);

	safeZeroTurn(90.0);

	safeDrawLine(60.0);

	safeZeroTurn(-90.0);

	safeDrawLine(40.0);

}


void drawB() {

	safeDrawLine(40);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(60);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(60);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(160);

}


void drawC() {

	safeDrawLine(40);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(160);	

}


void drawD() {

	safeDrawLine(40);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(160);

}


void drawE() {

	safeDrawLine(40);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(60);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(60);

	safeZeroTurn(-90);

	safeDrawLine(160);

}


void drawF() {

	safeDrawLine(40);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(100);

	safeZeroTurn(180);

	safeDrawLine(100);

	safeZeroTurn(-90);

	safeDrawLine(60);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(60);

	safeZeroTurn(-90);

	safeDrawLine(160);

}


void drawG() {

	safeDrawLine(40);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(60);

	safeZeroTurn(-90);

	safeDrawLine(30);

	safeZeroTurn(180);

	safeDrawLine(30);

	safeZeroTurn(90);

	safeDrawLine(60);

	safeZeroTurn(-90);

	safeDrawLine(40);

}


void drawH(){

	safeDrawLine(40);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(60);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(60);

	safeZeroTurn(180);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(40);	

}



void drawI(){

	safeDrawLine(100);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(60);

	safeZeroTurn(180);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(60);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(100);

}


void drawJ(){

	safeDrawLine(40);

	safeZeroTurn(-90);

	safeDrawLine(60);

	safeZeroTurn(180);

	safeDrawLine(60);

	safeZeroTurn(-90);

	safeDrawLine(60);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(60);

	safeZeroTurn(180);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(60);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(100);	

}


void drawK() {

	safeDrawLine(40);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(60);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(60);

	safeZeroTurn(180);

	safeDrawLine(60);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(-153.4);

	safeDrawLine(134.16);

	safeZeroTurn(-26.56);

	safeDrawLine(40);	

}


void drawL() {

	safeDrawLine(40);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(160);

}


void drawM(){

	safeDrawLine(40);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(60);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(60);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(40);

}


void drawN() {

	safeDrawLine(40);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(135);

	safeDrawLine(169.706);

	safeZeroTurn(-135);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(40);

}


void drawO() {

	safeDrawLine(40);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(135);

	safeDrawLine(169.706);

	safeZeroTurn(-135);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(40);

}


void drawP() {

	safeDrawLine(40);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(60);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(60);

	safeZeroTurn(-90);

	safeDrawLine(160);

}


void drawQ() {

	safeDrawLine(40);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(90);

	safeZeroTurn(45);

	safeDrawLine(50);

	safeZeroTurn(180);

	safeDrawLine(100);

	safeZeroTurn(180);

	safeDrawLine(50);

	safeZeroTurn(-45);

	safeDrawLine(70);

}


void drawR(){

	safeDrawLine(40);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(60);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(-153.43);

	safeDrawLine(134.16);

	safeZeroTurn(-26.56);

	safeDrawLine(40);

}


void drawS() {

	safeDrawLine(160);

	safeZeroTurn(-90);

	safeDrawLine(60);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(60);

	safeZeroTurn(90);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(60);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(90);

	safeDrawLine(60);

	safeZeroTurn(-90);

	safeDrawLine(40);	

}


void drawT() {

	safeDrawLine(100);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(60);

	safeZeroTurn(180);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(60);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(100);

}


void drawU() {

	safeDrawLine(40);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(120);

	safeZeroTurn(-90);

	safeDrawLine(40);

}


void drawV() {

	safeDrawLine(100);

	safeZeroTurn(-116.565);

	safeDrawLine(134.16);

	safeZeroTurn(180);

	safeDrawLine(134.16);

	safeZeroTurn(-126.870);

	safeDrawLine(134.16);

	safeZeroTurn(180);

	safeDrawLine(134.16);

	safeZeroTurn(-116.565);

	safeDrawLine(100);

}


void drawX() {

	safeDrawLine(40);

	safeZeroTurn(-45);

	safeDrawLine(169.706);

	safeZeroTurn(180);

	safeDrawLine(84.853);

	safeZeroTurn(90);

	safeDrawLine(84.853);

	safeZeroTurn(180);

	safeDrawLine(169.706);

	safeZeroTurn(-45);

	safeDrawLine(40);

}


void drawY() {

	safeDrawLine(100);

	safeZeroTurn(-90);

	safeDrawLine(60);

	safeZeroTurn(-45);

	safeDrawLine(84.853);

	safeZeroTurn(180);

	safeDrawLine(84.853);

	safeZeroTurn(-90);

	safeDrawLine(84.853);

	safeZeroTurn(180);

	safeDrawLine(84.853);

	safeZeroTurn(-45);

	safeDrawLine(60);

	safeZeroTurn(-90);

	safeDrawLine(100);

}


void drawZ() {

	safeDrawLine(40);

	safeZeroTurn(-45);

	safeDrawLine(169.706);

	safeZeroTurn(-135);

	safeDrawLine(120);

	safeZeroTurn(180);

	safeDrawLine(120);

	safeZeroTurn(135);

	safeDrawLine(169.706);

	safeZeroTurn(-135);

	safeDrawLine(160);

}


void drawString(char *string) {

	while(*string) {

		switch(*string) {

				case 'A':

					drawA();

				break;

				case 'E':

					drawE();

				break;

				case 'F':

					drawF();

				break;

				case 'H':

					drawH();

				break;

				case 'I':

					drawI();

				break;

				case 'J':

					drawJ();

				break;

				case 'K':

					drawK();

				break;

				case 'L':

					drawL();

				break;

				default:

					drawZ();

		}

		string++;

	}

	return;

}




int main() {
	//print("Drawing\n");
	drawA();
	//print("A\n");
	drawL();
	//print("L\n");
	drawI();
	//print("I\n");
	drawK();
	//print("K\n");
	drawE();
	//print("E\n");
	print("E\n");
	return 0;

	

	

}
