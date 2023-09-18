/*
 * movements.c
 *
 *  Created on: Jul 7, 2023
 *      Author: Ali Mohamed Taima
 */
#include "movements.h"
SERVO_t Servo1, Servo2, Servo3, Servo4, Servo5;

void init_servo(void)
{
	Servo1.Pin = Servo_PA15;
	Servo2.Pin = Servo_PB10;
	Servo3.Pin = Servo_PB14;
	Servo4.Pin = Servo_PB2;
	Servo5.Pin = Servo_PB9;
}
void one(void)
{
	Servo_void_SetAngle(Servo1, 100);	//Thumb
	Servo_void_SetAngle(Servo2, 40);	//Index
	Servo_void_SetAngle(Servo3, 170);	//Middle
	Servo_void_SetAngle(Servo4, 20);	//Ring
	Servo_void_SetAngle(Servo5, 120);	//Pinky
}
void two(void)
{
	Servo_void_SetAngle(Servo1, 100);	//Thumb
	Servo_void_SetAngle(Servo2, 40);	//Index
	Servo_void_SetAngle(Servo3, 50);	//Middle
	Servo_void_SetAngle(Servo4, 20);	//Ring
	Servo_void_SetAngle(Servo5, 120);	//Pinky
}
void three(void)
{
	Servo_void_SetAngle(Servo1, 100);	//Thumb
	Servo_void_SetAngle(Servo2, 40);	//Index
	Servo_void_SetAngle(Servo3, 50);	//Middle
	Servo_void_SetAngle(Servo4, 120);	//Ring
	Servo_void_SetAngle(Servo5, 120);	//Pinky
}
void four(void)
{
	Servo_void_SetAngle(Servo1, 100);	//Thumb
	Servo_void_SetAngle(Servo2, 40);	//Index
	Servo_void_SetAngle(Servo3, 50);	//Middle
	Servo_void_SetAngle(Servo4, 120);	//Ring
	Servo_void_SetAngle(Servo5, 50);	//Pinky
}
void good(void)
{
	Servo_void_SetAngle(Servo1, 170);	//Thumb
	Servo_void_SetAngle(Servo2, 120);	//Index
	Servo_void_SetAngle(Servo3, 170);	//Middle
	Servo_void_SetAngle(Servo4, 20);	//Ring
	Servo_void_SetAngle(Servo5, 120);	//Pinky
}
void ok(void)
{
	Servo_void_SetAngle(Servo1, 100);	//Thumb
	Servo_void_SetAngle(Servo2, 120);	//Index
	Servo_void_SetAngle(Servo3, 50);	//Middle
	Servo_void_SetAngle(Servo4, 120);	//Ring
	Servo_void_SetAngle(Servo5, 50);	//Pinky
}
void fest(void)
{
	Servo_void_SetAngle(Servo1, 100);	//Thumb
	Servo_void_SetAngle(Servo2, 120);	//Index
	Servo_void_SetAngle(Servo3, 170);	//Middle
	Servo_void_SetAngle(Servo4, 20);	//Ring
	Servo_void_SetAngle(Servo5, 120);	//Pinky
}
void rest(void)
{
	Servo_void_SetAngle(Servo1, 150);	//Thumb
	Servo_void_SetAngle(Servo2, 60);	//Index
	Servo_void_SetAngle(Servo3, 80);	//Middle
	Servo_void_SetAngle(Servo4, 80);	//Ring
	Servo_void_SetAngle(Servo5, 70);	//Pinky
}
void five(void)
{
	Servo_void_SetAngle(Servo1, 170);	//Thumb
	Servo_void_SetAngle(Servo2, 40);	//Index
	Servo_void_SetAngle(Servo3, 50);	//Middle
	Servo_void_SetAngle(Servo4, 120);	//Ring
	Servo_void_SetAngle(Servo5, 50);	//Pinky
}
void gun(void)
{
	Servo_void_SetAngle(Servo1, 170);	//Thumb
	Servo_void_SetAngle(Servo2, 40);	//Index
	Servo_void_SetAngle(Servo3, 190);	//Middle
	Servo_void_SetAngle(Servo4, 10);	//Ring
	Servo_void_SetAngle(Servo5, 140);	//Pinky
}

