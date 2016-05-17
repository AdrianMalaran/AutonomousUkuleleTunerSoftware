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

task main()
{eraseDisplay();
	while (true)
	{

		displayString(0, "Value = %d", average());
	}

}
