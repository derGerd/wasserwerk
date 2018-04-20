/*
 * Stoppuhr.cpp
 *
 *  Created on: 08.04.2018
 *      Author: Jan-Gerd
 */

#include "Stoppuhr.h"
#include <sstream>
#include <time.h>

Stoppuhr::Stoppuhr()
{
	// TODO Auto-generated constructor stub

}

Stoppuhr::~Stoppuhr()
{
	// TODO Auto-generated destructor stub
}

const string Stoppuhr::gibZeit(void)
{
	time_t Zeitstempel;
	tm *nun;
	Zeitstempel = time(0);
	nun = localtime(&Zeitstempel);
	stringstream ausgabe;
	ausgabe << nun->tm_mday << '.' << nun->tm_mon + 1 << '.'
			<< nun->tm_year + 1900 << "_" << nun->tm_hour << ':' << nun->tm_min
			<< ':' << nun->tm_sec;

	return ausgabe.str();
}


const uint64_t Stoppuhr::gibNanoSekunden(void)
{
	timespec ts;
	clock_gettime( CLOCK_MONOTONIC, &ts );
	return (uint64_t)ts.tv_sec * 1000000000U + (uint64_t)ts.tv_nsec;
}
