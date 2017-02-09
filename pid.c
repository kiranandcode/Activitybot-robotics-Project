#include <stdlib.h>
#include <stdio.h>
#include <time.h>

float setPoint;
float currentValue, lastValue, minValue, maxValue;
float lastError;
float integralSum;
float output;
float ki, kp, kd; 
clock_t lastTime = 0;
float timeInterval = 1000;
int initialized = 0;
int activated = 0;
int controlDirection = 0;
int irLeft, irRight;


void drive_speed(int left, int right) {
	printf("Driving forward speed %d, %d\n", left, right);
}

	
void initializePID(float target, float min, float max, float startOutput, float Ki, float Kd, float Kp, int controlDir ) {
	controlDirection = controlDir;

	setPoint = target;
	currentValue = startOutput;
	lastValue = currentValue;
	minValue = min;
	maxValue = max;
	ki = Ki; kd = Kd; kp = Kp;
	activated = 1;
	initialized = 1;
}


void proportionalIntegralDifferential() {
	clock_t currentTime = clock();
	float timeChange = currentTime - lastTime;
	if(!initialized || !activated) return;
	if(timeChange >= timeInterval) {
		int error = setPoint - currentValue;
		
		// Set/Update all the components for the calculation
		float proportionalComponent = error * kp;

		integralSum += ki*error;

		if(integralSum > maxValue) integralSum = maxValue;
		if(integralSum < minValue) integralSum = minValue;
		float derivativeComponent = (currentValue-lastValue);

		// Update values to be used in next iteration
		lastValue = currentValue;
		lastTime = currentTime; 
		output = proportionalComponent + integralSum - kd*derivativeComponent;


		if(output > maxValue) output = maxValue; if(output < minValue) output = minValue;
	}
}

void setConstants(float Ki, float Kp, float Kd) {
	if(Kp<0 || Ki<0 || Kd <0) return;
	float timeIntervalSec = timeInterval/1000;
	kp = Kp;
	ki = Ki * timeIntervalSec;
	kd = Kd / timeIntervalSec;

	if(controlDirection == 1) {
		kp = 0-kp;
		ki = 0-ki;
		kd = 0-kd;
	}
}

void setControlDirection(int direction) {
	if(direction != 0 || direction != 1) return;
	controlDirection = direction;
}

void setTimeInterval(float newTime){
	if(newTime > 0) {
		float ratio = newTime/timeInterval;
		ki *= ratio;
		kd /= ratio;
	}
}

void setOutputLimits(float min, float max) {
	if(min > maxValue) return;
	minValue = min;
	maxValue = max;

	if(output > maxValue) output = maxValue; 
	if(output < minValue) output = minValue;

	if(integralSum > maxValue) integralSum = maxValue; 
	if(integralSum < minValue) integralSum = minValue;
}

void internalReInitialize(){
	lastValue = currentValue;
	integralSum = output;
	if(integralSum > maxValue) integralSum = maxValue;
	if(integralSum < minValue) integralSum = minValue;
}

void activate() {
	if(activated) activated = 0;
	else activated = 1;
}


void setRobotSpeed(float inputValue) {
	int value = (int) inputValue;
	int left; int right;
	if(value > 50) {
		right = value;
		left = 100 - value; 
	}
	else {
		left = value;
		right = 100-value;
	}
	float ratio = 128.0/100;
	left = (float)left*ratio;
	right = (float)right*ratio;
	drive_speed(left, right);
}

void getSensorLeftResponse() {
	freqout(11,1,38000);
	irLeft = input(10);
}

void getSensorRightResponse() {
	freqout(1, 1, 38000);
	irRight = input(2);
}

void runPID() {
	int leftCurrentValue = 0, rightCurrentValue = 0;
	while(1) {
		getSensorLeftResponse();
		getSensorRightResponse();
		if(!irLeft) leftCurrentValue++;
		else if(leftCurrentValue > 0) leftCurrentValue--;
		if(!irRight) rightCurrentValue++;
		else if(rightCurrentValue > 0) rightCurrentValue--;
		int holdOutput = currentValue;
		currentValue = leftCurrentValue;
		proportionalIntegralDifferential();
		currentValue = rightCurrentValue;
		proportionalIntegralDifferential();
	}
}

void wallFollow() {
	initializePID(10, 0, 100, 50, 10, 10, 10, 0);
	start_cog(runPID, 128);
	while(1) {
		setRobotSpeed(output);
	}	
}



int main() {
	while(1) {
		currentValue += output;
		proportionalIntegralDifferential();

	}

}


