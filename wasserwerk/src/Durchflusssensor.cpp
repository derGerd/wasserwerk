/*
 * Durchflusssensor.cpp
 *
 *  Created on: 08.04.2018
 *      Author: Jan-Gerd
 */

#include "Durchflusssensor.h"
#include "pigpio.h"
#include <iostream>
#include <time.h>
struct timespec ts;

Durchflusssensor::geschwindigkeit_t Durchflusssensor::geschwindigkeit;
Durchflusssensor::tick_t Durchflusssensor::letzterTick;

Durchflusssensor::Durchflusssensor()
{

	gpioSetMode(HALL, PI_INPUT);

	gpioSetPullUpDown(HALL, PI_PUD_UP);

	gpioSetAlertFunc(HALL, hallPuls);

}

Durchflusssensor::~Durchflusssensor()
{
}

void Durchflusssensor::hallPuls(int gpio, int level, uint32_t tick)
{
	cout << " hallPuls Pin:" << gpio << " level:" << level << " tick:" << tick
			<< endl;

	if (Durchflusssensor::letzterTick[gpio])
	{
		float differenz = (tick - Durchflusssensor::letzterTick[gpio]);

		if (differenz)
		{
			Durchflusssensor::geschwindigkeit[gpio] = 700000000L / differenz;
		}

		cout << "Differenz" << differenz << " Geschwindigkeit:"
				<< Durchflusssensor::geschwindigkeit[gpio] << endl;
	}
	Durchflusssensor::letzterTick[gpio] = tick;
}
