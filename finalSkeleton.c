void start();
int pluck(char note);
void moveTo(char note);
bool inTune(int tune);
void flatten(int direction);
void rotate(int direction);
void reset();
int average();

task main()
{
	nMotorEncoder[motorA] = 0; // pluck
	SensorType[S2] = sensorColorNxtFULL;
	eraseDisplay();

	while(true)
	{
		if(!inTune(pluck('G')))
			displayString(0,"Not in tune");
		if(inTune(pluck('G')))
			displayString(0,"In tune");
		eraseDisplay();
	}

}

void start()
{
	while(nNxtButtonPressed != 3)
	{}
	while(nNxtButtonPressed == 3)
	{}
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
   	wait1Msec(2000);

		while(nMotorEncoder[motorA] > 25)
   		motor[motorA] = -5;

   	motor[motorA] = 0;
   	wait1Msec(2000);

		return average();
	}

	if(note == 'E')
	{
    while(nMotorEncoder[motorA] < 120)
  		motor[motorA] = 5;

  	motor[motorA] = 0;
  	wait1Msec(2000);

		while(nMotorEncoder[motorA] > 55)
   		motor[motorA] = -5;

   	motor[motorA] = 0;
   	wait1Msec(2000);

		return average();
	}

	if(note == 'A')
	{
    while(nMotorEncoder[motorA] < 130)
    	motor[motorA] = 5;

    motor[motorA] = 0;
    wait1Msec(2000);

		while(nMotorEncoder[motorA] > 90)
    	motor[motorA] = -5;

    motor[motorA] = 0;
    wait1Msec(2000);

		return average();
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

	else
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
   if(tune != 5)
     return true;
   return false;
}

void flatten(int direction)
{
	if(direction == 1)
	{
		//mux motor code
		if(SensorValue[S1] == 1)
			return;
	}

	if(direction == -1)
	{
    //mux motor code
		if(SensorValue[S1] == 1)
			return;
	}
}

void rotate(int direction)
{
	if(direction == 1)
	{
    //mux motor code
		if(SensorValue[S1] == 1)
			return;
	}

	if(direction == -1)
	{
    //mux motor code
		if(SensorValue[S1] == 1)
			return;
	}
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
