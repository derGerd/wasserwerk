/*
 * WebStartseite.h
 *
 *  Created on: 08.04.2018
 *      Author: Jan-Gerd
 */

#ifndef WEBSTARTSEITE_H_
#define WEBSTARTSEITE_H_

#include "include/CivetServer.h"
#include "TemperaturSensor.h"
#define EXIT_URI "/exit"

class WebStartseite: public CivetHandler
{
private:
	TemperaturSensor & ts;

public:

	WebStartseite(TemperaturSensor & _ts);
	virtual ~WebStartseite();

	bool handleGet(CivetServer *server, struct mg_connection *conn);


};

#endif /* WEBSTARTSEITE_H_ */
