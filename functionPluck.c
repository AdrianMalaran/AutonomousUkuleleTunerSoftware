int pluck(char note);

task main()
{

	nMotorEncoder[motorA] = 0;

	pluck('G');
	pluck('C');
	pluck('E');
	pluck('A');

}

int pluck(char note)
{
	if(note == 'G')
	{
    while(nMotorEncoder[motorA] < 60)
    	motor[motorA] = 5;

   	motor[motorA] = 0;
    wait1Msec(2000);

		while(nMotorEncoder[motorA] > -5)
    	motor[motorA] = -5;

		motor[motorA] = 0;
    wait1Msec(2000);

		return SensorValue[S2];
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

		return SensorValue[S2];
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

		return SensorValue[S2];
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

		return SensorValue[S2];
	}

	return -1;
}
