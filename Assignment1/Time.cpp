#include "Time.h"


Time::Time() {
	int size = 1;
	index = 0;
	dynTemp = new int[size];
	tempTemp = nullptr;
	counter = 0;
}

Time::Time(int temperature) {
	index = 0;
	counter = 1;
	int size = 1;
	tempTemp = nullptr;
	dynTemp = new int[size];
	dynTemp[index] = temperature;
	index++;
}

void Time::addTemp(int temperature) {
	dynTemp[index] = temperature;
	counter++;
	index++;
	tempTemp = new int[counter];
	for (unsigned int i = 0; i < counter; i++) {
		tempTemp[i] = dynTemp[i];
	}
	delete[] dynTemp;
	dynTemp = nullptr;
	dynTemp = tempTemp;
	tempTemp = nullptr;
}

double Time::getMean() {
	double sum = 0;
	for (unsigned int i = 0; i < counter; i++) {
		sum += dynTemp[i];
	}
	return sum / counter;
}