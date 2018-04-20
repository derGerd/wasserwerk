/*
 * Stoppuhr.h
 *
 *  Created on: 08.04.2018
 *      Author: Jan-Gerd
 */

#ifndef STOPPUHR_H_
#define STOPPUHR_H_

#include <time.h>
#include <string>

using namespace std;

class Stoppuhr
{
public:
	Stoppuhr();
	virtual ~Stoppuhr();

	static const string gibZeit(void);

	static const uint64_t gibNanoSekunden(void);
};

#endif /* STOPPUHR_H_ */
