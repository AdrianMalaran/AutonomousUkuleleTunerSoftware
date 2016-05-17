#include "mindsensors-motormux.h"

void start();
int pluck(char note);
void moveTo(char note);
bool inTune(int tune);
void flatten(int direction);
void rotate(int direction);
int average();
void reset();


task main()
{
	nMotorEncoder[motorA] = 0; // pluck
	nMotorEncoder[motorB] = 0; // forward
	nMotorEncoder[motorC] = 0; // left/right
	SensorType[S3] = sensorI2CCustom; // rotate
	MSMMUXinit();
	SensorType[S1] = sensorTouch;
	SensorType[S2] = sensorColorNxtFULL;

	start();
}

void start()
{
	eraseDisplay();
	displayString(0, "Arrows for Rotate");
	while(nNxtButtonPressed != 3)
	{
		while(nNxtButtonPressed == 1)
		{
			MSMMotor(mmotor_S3_1, 100);
		}
		while(nNxtButtonPressed == 2)
		{
			MSMMotor(mmotor_S3_1, 100);
		}

		MSMotorStop(mmotor_S3_1);
	}
	while(nNxtButtonPressed == 3)
	{}

	eraseDisplay();
	displayString(0, "Arrows for L/R");
	while(nNxtButtonPressed != 3)
	{
		while(nNxtButtonPressed == 1)
		{
			motor[motorB] = 5;
		}
		while(nNxtButtonPressed == 2)
		{
			motor[motorB] = -5;
		}

		motor[motorB] = 0;
	}
	while(nNxtButtonPressed == 3)
	{}

	eraseDisplay();
}

int pluck(char note)
{
	int a1 = 0;
	int a2 = 0;
	if(note == 'G')
	{
    while(nMotorEncoder[motorA] < 60)
    	motor[motorA] = 5;

   	motor[motorA] = 0;
   	a1 = average();

		while(nMotorEncoder[motorA] > -5)
    	motor[motorA] = -5;

		motor[motorA] = 0;
		a2 = average();

		return (a1+a2)/2;
	}

	if(note == 'C')
	{
    while(nMotorEncoder[motorA] < 90)
   		motor[motorA] = 5;

   	motor[motorA] = 0;
   	a1 = average();

		while(nMotorEncoder[motorA] > 25)
   		motor[motorA] = -5;

   	motor[motorA] = 0;
   	a2 = average();

		return (a1+a2)/2;
	}

	if(note == 'E')
	{
    while(nMotorEncoder[motorA] < 120)
  		motor[motorA] = 5;

  	motor[motorA] = 0;
  	a1 = average();

		while(nMotorEncoder[motorA] > 55)
   		motor[motorA] = -5;

   	motor[motorA] = 0;
   	a2 = average();

		return (a1+a2)/2;
	}

	if(note == 'A')
	{
    while(nMotorEncoder[motorA] < 130)
    	motor[motorA] = 5;

    motor[motorA] = 0;
    a1 = average();

		while(nMotorEncoder[motorA] > 90)
    	motor[motorA] = -5;

    motor[motorA] = 0;
    a2 = average();

		return (a1+a2)/2;
	}

	return -1;
}

void moveTo(char note)
{

	if(note == 'C')
	{
    while(nMotorEncoder[motorC] > 15)
    motor[motorC] = -25;

		while(nMotorEncoder[motorB] < 10)
    motor[motorB] = 25;

		while(nMotorEncoder[motorC] < 30)
		{
			rotate(1);
    	motor[motorC] = 25;
    }
	}

	else if(note == 'E')
	{
    while(nMotorEncoder[motorC] > 0)
    {
    	rotate(1);
    	motor[motorC] = -25;
    }
	}

	else if(note == 'A')
	{
    while(nMotorEncoder[motorC]  < 15)
    motor[motorC] = 25;

		while(nMotorEncoder[motorB] < 10)
    motor[motorB] = 25;

		while(nMotorEncoder[motorC] > 0)
		{
			rotate(1);
    	motor[motorC] = -25;
    }
	}
}

bool inTune(int tune)
{
   if(tune == 1)
     return true;
   return false;
}

void flatten(int direction)
{
		MSMMotor(mmotor_S3_1, 5 * direction);
		wait1Msec(500);
		MSMotorStop(mmotor_S3_1);
}

void rotate(int direction)
{
  	MSMMotor(mmotor_S3_1, 5 * direction);
  	wait1Msec(100);
  	MSMotorStop(mmotor_S3_1);
}

void reset()
{
  while(nMotorEncoder[motorC] < 15)
		motor[motorC] = 25;

	while(nMotorEncoder[motorB] > 0)
		motor[motorB] = -25;
}

int average()
{
	SensorType[S2] = sensorColorNxtFULL;
	clearTimer(T1);
	int count = 0;
	int totalValue = 0;
	while(time1[T1] < 100)
	{
		totalValue += SensorValue[S2];
		count++;
	}

	return ceil(1.0*totalValue/count);
}
