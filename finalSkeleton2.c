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
	nMotorEncoder[motorB] = 0; // left/right
	nMotorEncoder[motorC] = 0; // forward
	SensorType[S3] = sensorI2CCustom; // rotate
	MSMMUXinit();


	char g = 'G', c = 'C', e = 'E', a = 'A';

	start();

	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorB] = 0;
	nMotorEncoder[motorC] = 0;
	MSMMotorEncoderReset(mmotor_S3_1);


	moveTo(c);
	moveTo(e);
	moveTo(a);

	eraseDisplay();
	displayString(0, "Press orange to");
	displayString(1, "reset"):
	while(nNxtButtonPressed != 3)
	{}
	reset();
	while(nNxtButtonPressed == 3)
	{}

	MSMotorStop(mmotor_S3_1);
}

void start()
{
	bool set = false;
	while(set == false)
	{
		eraseDisplay();
		displayString(0, "Arrows for F/R");
		while(nNxtButtonPressed != 3)
		{
			while(nNxtButtonPressed == 1)
			{
				motor[motorC] = 10;
				eraseDisplay();
			}
			while(nNxtButtonPressed == 2)
			{
				motor[motorC] = -10;
				eraseDisplay();
			}

			displayString(1, "F/R = %d", nMotorEncoder[motorC]);

			motor[motorC] = 0;
		}
		while(nNxtButtonPressed == 3)
		{}

		eraseDisplay();
		displayString(0, "Arrows for Rotate");
		while(nNxtButtonPressed != 3)
		{
			while(nNxtButtonPressed == 1)
			{
				MSMMotor(mmotor_S3_1, 20);
			}
			while(nNxtButtonPressed == 2)
			{
				MSMMotor(mmotor_S3_1, -20);
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
				motor[motorB] = 10;
				eraseDisplay();
			}
			while(nNxtButtonPressed == 2)
			{
				motor[motorB] = -10;
				eraseDisplay();
			}
			displayString(1, "L/R = %d", nMotorEncoder[motorB]);
			motor[motorB] = 0;
		}
		while(nNxtButtonPressed == 3)
		{}

		eraseDisplay();
		displayString(0, "Press orange");
		displayString(1, "to confirm");
		displayString(2, "arows to retry");
		while(nNxtButtonPressed == -1)
		{}
		while(nNxtButtonPressed != -1)
		{
			if(nNxtButtonPressed == 3)
				set = true;
		}
		eraseDisplay();
	}
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
		while(nMotorEncoder[motorB] < 70)
			motor[motorB] = 10;
		motor[motorB] = 0;
		while(nMotorEncoder[motorC] < 150)
			motor[motorC] = 20;
		motor[motorC] = 0;
		while(nMotorEncoder[motorB] > 60)
			motor[motorB] = -10;
		motor[motorB] = 0;
		while(nMotorEncoder[motorB] > 0)
		{
			motor[motorB] = -5;
		}
		motor[motorB] = 0;
	}

	else if(note == 'E')
	{
		while(nMotorEncoder[motorB] < 120)
			motor[motorB] = 10;
		motor[motorB] = 0;

		while(nMotorEncoder[motorB] < 170)
		{
			motor[motorB] = 5;
		}
		motor[motorB] = 0;
	}

	else if(note == 'A')
	{
		while(nMotorEncoder[motorB]  > 100)
			motor[motorB] = -10;
		motor[motorB] = 0;

		while(nMotorEncoder[motorC] > -5)
			motor[motorC] = -20;
		motor[motorC] = 0;

		while(nMotorEncoder[motorB] < 180)
		{
			motor[motorB] = 10;
		}
		motor[motorB] = 0;
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
	MSMMotor(mmotor_S3_1, 20 * direction);
	wait1Msec(1500);
	MSMotorStop(mmotor_S3_1);
}

void rotate(int direction)
{
	MSMMotor(mmotor_S3_1, 20 * direction);
	wait1Msec(500);
	MSMotorStop(mmotor_S3_1);
}

void reset()
{
	while(nMotorEncoder[motorB] > 100)
		motor[motorB] = -10;
	motor[motorB] = 0;

	while(nMotorEncoder[motorC] < 150)
		motor[motorC] = 10;
	motor[motorC] = 0;
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
