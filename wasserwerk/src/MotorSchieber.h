/*
 * MotorSchieber.h
 *
 *  Created on: 10.04.2018
 *      Author: Jan-Gerd
 */

#ifndef MOTORSCHIEBER_H_
#define MOTORSCHIEBER_H_

#include <cstdlib>
#include <cstdint>
#include "include/CivetServer.h"
#include "pigpio.h"

class MotorSchieber
{
private:
	static constexpr uint32_t SCHIEBER_AUF = 23;
	static constexpr uint32_t SCHIEBER_ZU = 24;

public:
	MotorSchieber();
	virtual ~MotorSchieber();

	void schieber_auf(void)
	{
		gpioWrite(SCHIEBER_AUF, 1);
		gpioWrite(SCHIEBER_ZU, 0);
	}

	void schieber_zu(void)
	{
		gpioWrite(SCHIEBER_ZU, 1);
		gpioWrite(SCHIEBER_AUF, 0);
	}

	void schieber_stopp(void)
	{
		gpioWrite(SCHIEBER_AUF, 0);
		gpioWrite(SCHIEBER_ZU, 0);
	}
};

#endif /* MOTORSCHIEBER_H_ */
