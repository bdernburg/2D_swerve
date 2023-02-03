/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
 
//variable for using ime counts
int counts;

float target_dist = 30;
int tempDistance0;
int tempDistance1;
float speed;
//int direction;
float kp = 1.4;
float turnkp = 10;

void follow2D(int distance0, int distance1){
	int avg_Dist = (distance0 + distance1)/2;
	if (abs(distance0-distance1)) {
		motorSet(6, kp*(45+(imeGet(IME_MOTOR_1, &counts)*27/79)));
		motorSet(7, kp*(-45+(imeGet(IME_MOTOR_2, &counts)*27/79)));
		motorSet(8, kp*(-45+(imeGet(IME_MOTOR_3, &counts)*27/79)));
		motorSet(9, kp*(45+(imeGet(IME_MOTOR_4, &counts)*27/79)));

		speed = turnkp * (distance0-distance1);
		motorSet(2, speed);
		motorSet(3, speed);
		motorSet(4, speed);
		motorSet(5, speed);
	}
	if (avg_Dist < (target_dist - 3) || (avg_Dist > (target_dist + 3))) {
		speed =  (abs(avg_Dist - target_dist)) * kp;
		motorSet(2, speed);
		motorSet(3, speed);
		motorSet(4, speed);
		motorSet(5, speed);
	}
	else {
		motorStopAll();
	}
	/*
	if difference between distances from sensors is too extreme {
		proportional control turn
	}
	if average of 2 distances are out of zone{
		move straight
	}
	else {
		stop
	}
	*/
	
}

void zero(){
	if(joystickGetDigital(1, 8, JOY_UP) == 1){	
		imeReset(IME_MOTOR_1);
		imeReset(IME_MOTOR_2);
		imeReset(IME_MOTOR_3);
		imeReset(IME_MOTOR_4);
	}
}

void operatorControl()
{
	Ultrasonic usonic0;
	usonic0 = ultrasonicInit(1, 2);
	Ultrasonic usonic1;
	usonic1 = ultrasonicInit(1, 2);
	while(true){
		tempDistance0 = ultrasonicGet(usonic0);
		tempDistance1 = ultrasonicGet(usonic1);
		follow2D(tempDistance0, tempDistance1);
		zero();
		wait(0.05);
	}
}
