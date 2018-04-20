/*
 * Durchflusssensor.h
 *
 *  Created on: 08.04.2018
 *      Author: Jan-Gerd
 */

#ifndef DURCHFLUSSSENSOR_H_
#define DURCHFLUSSSENSOR_H_

#include "pigpio.h"
#include <map>

using namespace std;

class Durchflusssensor
{
private:
	static constexpr uint32_t HALL = 18;
	static void hallPuls(int gpio, int level, uint32_t tick);

public:
	typedef map<uint32_t, float> geschwindigkeit_t;
	typedef map<uint32_t, uint64_t> tick_t;
	static geschwindigkeit_t geschwindigkeit;
	static tick_t letzterTick;

	Durchflusssensor();
	virtual ~Durchflusssensor();

};

#endif /* DURCHFLUSSSENSOR_H_ */
