#pragma once
class Time
{
private:
	int* dynTemp;
	int* tempTemp;
	//int temperature;
	int counter;
	int index;

public:
	Time();
	Time(int temperature);
	void addTemp(int temperature);
	double getMean();

};

