#include "mindsensors-motormux.h"

//Initial function declerations
void start();
int pluck(char note);
void moveTo(char note);
bool inTune(int tune);
void rotate(int direction);
int average();
void reset();


task main()
{
	//Initialaze sensors and reset encoder values
	nMotorEncoder[motorA] = 0; // pluck
	nMotorEncoder[motorB] = 0; // forward
	nMotorEncoder[motorC] = 0; // left/right
	SensorType[S3] = sensorI2CCustom; // rotate   Multiplexer in port 3
	MSMMUXinit();
	SensorType[S1] = sensorTouch; // Touch sensor in port 1
	SensorType[S2] = sensorColorNxtFULL; // Colour sensor in port 2

	char g = 'G', c = 'C', e = 'E', a = 'A';//variables for each string

	//Function in order to set relative position
	//(tuner on G default) of system using arrows on brick
	start();

	displayString(0, "Tuning");

	//Reset encoder values after position has been set
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorB] = 0;
	nMotorEncoder[motorC] = 0;
	MSMMotorEncoderReset(mmotor_S3_1);

	//loop while the g string is not in tune
	//pluck returns an integer which is compared to
	//a default value then a bool is returned (functions below)
	while(!inTune(pluck(g)))
	{

		rotate(-20);	//while not in tune, rotate the peg (function below)
		if(SensorValue[S1] == 1)//if while rotating the touch is pressed, stop rotating
			return;
	}

	rotate(10);//rotate in the opposite direction to make it easier to disengage the tuning peg
	moveTo(c);//move to the c string (see function below)
	pluck(c);//initial pluck in order for tuner to reset

	//loop while the c string is not in tune
	while(!inTune(pluck(c)))
	{

		rotate(-20);//rotate the peg
		if(SensorValue[S1] == 1)//if touch is pressed, stop
			return;
	}

	rotate(10);//rotate in the opposite direction to make it easier to disengage the tuning peg
	moveTo(e);//move to the e string
	pluck(e);//initial pluck in order for tuner to reset

	//loop while the e string is not in tune
	while(!inTune(pluck(e)))
	{
		rotate(20); //rotate the peg
		if(SensorValue[S1] == 1)//if touch is pressed, stop
			return;
	}

	rotate(-10);//rotate in the opposite direction to make it easier to disengage the tuning peg
	moveTo(a);//move to the a string
	pluck(a);//initial pluck in order for tuner to reset

	//loop while the a string is not in tune
	while(!inTune(pluck(a)))
	{
		rotate(20);//rotate the peg
		if(SensorValue[S1] == 1)//if touch is pressed, stop
			return;
	}

	rotate(-10);//rotate in the opposite direction to make it easier to disengage the tuning peg

	reset();//MOve the sytem to a position so that all tuning pegs are disengaged

	MSMotorStop(mmotor_S3_1);//turn off the multiplexer motor if still on
}

void start()
{
	bool set = false; //boolean acting as a yes/no for if the tuner is in the correct position
	while(set == false) //while the tuner is not in the correct position
	{
		//Use the arrows to position to move the system forward or backwards
		eraseDisplay();
		displayString(0, "Arrows for F/R");
		while(nNxtButtonPressed != 3)
		{
			while(nNxtButtonPressed == 1)
			{
				motor[motorC] = 10;
			}
			while(nNxtButtonPressed == 2)
			{
				motor[motorC] = -10;
			}

			motor[motorC] = 0;
		}
		while(nNxtButtonPressed == 3) //wait for release
		{}

		//Use the arrows to rotate the tuning device
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
		while(nNxtButtonPressed == 3) //wait for release
		{}

		//Use the arrows to move the system left or right
		eraseDisplay();
		displayString(0, "Arrows for L/R");
		while(nNxtButtonPressed != 3)
		{
			while(nNxtButtonPressed == 1)
			{
				motor[motorB] = 10;
			}
			while(nNxtButtonPressed == 2)
			{
				motor[motorB] = -10;
			}

			motor[motorB] = 0;
		}
		while(nNxtButtonPressed == 3) //wait for release
		{}

		//Use the arrows to move the plucking device left or right
		eraseDisplay();
		displayString(0, "Arrows for L/R");
		displayString(1, "Plucker");
		while(nNxtButtonPressed != 3)
		{
			while(nNxtButtonPressed == 1)
			{
				motor[motorA] = 10;
			}
			while(nNxtButtonPressed == 2)
			{
				motor[motorA] = -10;
			}

			motor[motorA] = 0;
		}
		while(nNxtButtonPressed == 3) //wait for release
		{}

		eraseDisplay();
		displayString(0, "Press orange");
		displayString(1, "to confirm");
		displayString(2, "arows to retry");
		while(nNxtButtonPressed == -1) //wait for a button press
		{}
		while(nNxtButtonPressed != -1)
		{
			if(nNxtButtonPressed == 3) //if the orange button is pressed, switch the bool variable set to yes
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
			motor[motorA] = 10;

		motor[motorA] = 0;
		a1 = average();//store the first average sensor value

		//pluck the string going backwards using encoders
		while(nMotorEncoder[motorA] > -5)
			motor[motorA] = -10;

		motor[motorA] = 0;
		a2 = average();//store the second average sensor value

		return (a1+a2)/2;//return the average of both stored values
	}

	if(note == 'C')//check which note is pressed
	{
		//pluck the string going forwards using encoders
		while(nMotorEncoder[motorA] < 90)
			motor[motorA] = 10;

		motor[motorA] = 0;
		a1 = average();//store the first average sensor value

		//pluck the string going backwards using encoders
		while(nMotorEncoder[motorA] > 25)
			motor[motorA] = -10;

		motor[motorA] = 0;
		a2 = average();//store the second average sensor value

		return (a1+a2)/2;//return the average of both stored values
	}

	if(note == 'E')//check which note is pressed
	{
		//pluck the string going forwards using encoders
		while(nMotorEncoder[motorA] < 115)
			motor[motorA] = 10;

		motor[motorA] = 0;
		a1 = average();//store the first average sensor value

		//pluck the string going backwards using encoders
		while(nMotorEncoder[motorA] > 55)
			motor[motorA] = -10;

		motor[motorA] = 0;
		a2 = average();//store the second average sensor value

		return (a1+a2)/2;//return the average of both stored values
	}

	if(note == 'A')//check which note is pressed
	{
		//pluck the string going forwards using encoders
		while(nMotorEncoder[motorA] < 135)
			motor[motorA] = 10;

		motor[motorA] = 0;
		a1 = average();//store the first average sensor value

		//pluck the string going backwards using encoders
		while(nMotorEncoder[motorA] > 90)
			motor[motorA] = -10;

		motor[motorA] = 0;
		a2 = average();//store the second average sensor value

		return (a1+a2)/2;//return the average of both stored values
	}

	return -1;
}

void moveTo(char note)
{

	if(note == 'C') //Use encoder values to move to the C peg
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

		//just before engaging the peg, slow down and rotate in order to engage the peg
		while(nMotorEncoder[motorB] > 0)
		{
			rotate(30);
			motor[motorB] = -5;
		}
		motor[motorB] = 0;
	}

	else if(note == 'E') //Use encoder values to move to the E peg
	{
		while(nMotorEncoder[motorB] < 120)
			motor[motorB] = 10;
		motor[motorB] = 0;

		//just before engaging the peg, slow down and rotate in order to engage the peg
		while(nMotorEncoder[motorB] < 170)
		{
			rotate(30);
			motor[motorB] = 5;
		}
		motor[motorB] = 0;
	}

	else if(note == 'A') //Use encoder values to move to the A peg
	{
		while(nMotorEncoder[motorB]  > 110)
			motor[motorB] = -10;
		motor[motorB] = 0;

		while(nMotorEncoder[motorC] > -5)
			motor[motorC] = -20;
		motor[motorC] = 0;

		//just before engaging the peg, slow down and rotate in order to engage the peg
		while(nMotorEncoder[motorB] < 190)
		{
			rotate(-40);
			motor[motorB] = 10;
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

void rotate(int direction)
{
	//rotate the tuning peg to tune the ukulele
	MSMMotor(mmotor_S3_1, 1 * direction);
	wait1Msec(150);
	MSMotorStop(mmotor_S3_1);
}

void reset()
{
	//use encoder values to move the system to a position so that the ukulele can be easily removed
	while(nMotorEncoder[motorB] > 100)
		motor[motorB] = -10;
	motor[motorB] = 0;

	while(nMotorEncoder[motorC] < 150)
		motor[motorC] = 10;
	motor[motorC] = 0;

	while(nMotorEncoder[motorA] > 0)
		motor[motorA] = -10;
	motor[motorA] = 0;
}

int average()
{
	clearTimer(T1); //reset the timer

	//temp variable declarations
	int count = 0;
	int totalValue = 0;

	while(time1[T1] < 200)//loop over a time interval using timer
	{
		totalValue += SensorValue[S2];//total the sensor values recorded over the time interval
		count++;//increase the count each iteration (each time a value is added to the total)
	}

	return ceil(1.0*totalValue/count);//return the rounded average of the sensor over the time interval
}
