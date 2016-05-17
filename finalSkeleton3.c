#include "mindsensors-motormux.h"

//Initial function declerations
void start();
int pluck(char note);
void moveTo(char note);
bool inTune(int tune);
void rotateMove(int direction);
void rotate(int direction);
int average();
void reset();


task main()
{
	//Initialaze sensors and reset encoder values
	nMotorEncoder[motorA] = 0; // pluck
	nMotorEncoder[motorB] = 0; // forward
	nMotorEncoder[motorC] = 0; // left/right
	SensorType[S3] = sensorI2CCustom; // rotate
	MSMMUXinit();
	SensorType[S1] = sensorTouch;
	SensorType[S2] = sensorColorNxtFULL;

	char g = 'G', c = 'C', e = 'E', a = 'A';//variables for each string

	//Function in order to set relative position
	//(tuner on G default) of system using arrows on brick
	start();

	//Reset encoder values after position has been set
	nMotorEncoder[motorB] = 0;
	nMotorEncoder[motorC] = 0;
	MSMMotorEncoderReset(mmotor_S3_1);

	//loop while the g string is not in tune
	//pluck returns an integer which is compared to
	//a default value then a bool is returned (functions below)
	while(!inTune(pluck(g)))
	{
		eraseDisplay();
		displayString(0, "NOT IN TUNE");
		rotate(-1);	//while not in tune, rotate the peg (function below)
		if(SensorValue[S1] == 1)//if while rotating the touch is pressed, stop rotating
			return;
	}

	MSMotorStop(mmotor_S3_1);//turn off the multiplexer motor if still on
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
	//temp variable declaration
	int a1 = 0;
	int a2 = 0;
	if(note == 'G')//check which note is pressed
	{
		//pluck the string going forwards using encoders
		while(nMotorEncoder[motorA] < 60)
			motor[motorA] = 5;

		motor[motorA] = 0;
		a1 = average();//store the first average sensor value

		//pluck the string going backwards using encoders
		while(nMotorEncoder[motorA] > -5)
			motor[motorA] = -5;

		motor[motorA] = 0;
		a2 = average();//store the second average sensor value

		return (a1+a2)/2;//return the average of both stored values
	}

	if(note == 'C')//check which note is pressed
	{
		//pluck the string going forwards using encoders
		while(nMotorEncoder[motorA] < 90)
			motor[motorA] = 5;

		motor[motorA] = 0;
		a1 = average();//store the first average sensor value

		//pluck the string going backwards using encoders
		while(nMotorEncoder[motorA] > 25)
			motor[motorA] = -5;

		motor[motorA] = 0;
		a2 = average();//store the second average sensor value

		return (a1+a2)/2;//return the average of both stored values
	}

	if(note == 'E')//check which note is pressed
	{
		//pluck the string going forwards using encoders
		while(nMotorEncoder[motorA] < 120)
			motor[motorA] = 5;

		motor[motorA] = 0;
		a1 = average();//store the first average sensor value

		//pluck the string going backwards using encoders
		while(nMotorEncoder[motorA] > 55)
			motor[motorA] = -5;

		motor[motorA] = 0;
		a2 = average();//store the second average sensor value

		return (a1+a2)/2;//return the average of both stored values
	}

	if(note == 'A')//check which note is pressed
	{
		//pluck the string going forwards using encoders
		while(nMotorEncoder[motorA] < 130)
			motor[motorA] = 5;

		motor[motorA] = 0;
		a1 = average();//store the first average sensor value

		//pluck the string going backwards using encoders
		while(nMotorEncoder[motorA] > 90)
			motor[motorA] = -5;

		motor[motorA] = 0;
		a2 = average();//store the second average sensor value

		return (a1+a2)/2;//return the average of both stored values
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
			motor[motorC] = 10;
		motor[motorC] = 0;
		while(nMotorEncoder[motorB] > 0)
		{
			motor[motorB] = -5;
			rotateMove(2);
		}
		motor[motorB] = 0;
	}

	else if(note == 'E')
	{
		while(nMotorEncoder[motorB] < 120)
			motor[motorB] = 10;
		motor[motorB] = 0;

		while(nMotorEncoder[motorB] < 185)
		{
			motor[motorB] = 5;
			rotateMove(-2);
		}
		motor[motorB] = 0;
	}

	else if(note == 'A')
	{
		while(nMotorEncoder[motorB]  > 100)
			motor[motorB] = -10;
		motor[motorB] = 0;

		while(nMotorEncoder[motorC] > -5)
			motor[motorC] = -10;
		motor[motorC] = 0;

		while(nMotorEncoder[motorB] < 185)
		{
			motor[motorB] = 10;
			rotateMove(-2);
		}
		motor[motorB] = 0;
	}
}

bool inTune(int tune)
{
	if(tune != 5)//if the average sensor value over a time interval is not 5 (red) return true
		return true;
	return false;//otherwise false
}

void rotateMove(int direction)
{
	//rotate the tuning peg to tune the ukulele
	while(MSMMotorEncoder(mmotor_S3_1) < 300)
		MSMMotor(mmotor_S3_1, 20 * direction);

	MSMotorStop(mmotor_S3_1);
}

void rotate(int direction)
{
	//rotate the tuning peg to tune the ukulele
	MSMMotor(mmotor_S3_1, 30 * direction);
	wait1Msec(100);
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
	clearTimer(T1); //reset the timer

	//temp variable declarations
	int count = 0;
	int totalValue = 0;

	while(time1[T1] < 100)//loop over a time interval using timer
	{
		totalValue += SensorValue[S2];//total the sensor values recorded over the time interval
		count++;//increase the count each iteration (each time a value is added to the total)
	}

	return ceil(1.0*totalValue/count);//return the rounded average of the sensor over the time interval
}
