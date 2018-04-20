/*
 * TemperaturSensor.h
 *
 *  Created on: 08.04.2018
 *      Author: Jan-Gerd
 */

#ifndef TEMPERATURSENSOR_H_
#define TEMPERATURSENSOR_H_

#include <string>
#include <thread>
#include <mutex>
#include <deque>

using namespace std;

class TemperaturSensor
{
private:
//	string sensorpfad;

	thread hole_temperatur_thread;

	bool temperatur_holen;

public:
	mutex temperatur_mutex;
	deque<pair<string,float>> temperatur;

	TemperaturSensor();
	virtual ~TemperaturSensor();

	float gibTemperatur(void);

	static void hole_temperatur(TemperaturSensor* ts);
};

#endif /* TEMPERATURSENSOR_H_ */
