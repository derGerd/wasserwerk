/*
 * MotorSchieber.cpp
 *
 *  Created on: 10.04.2018
 *      Author: Jan-Gerd
 */

#include "MotorSchieber.h"

MotorSchieber::MotorSchieber()
{
	gpioSetMode(SCHIEBER_AUF, PI_OUTPUT);
	gpioSetMode(SCHIEBER_ZU, PI_OUTPUT);
}

MotorSchieber::~MotorSchieber()
{
}

