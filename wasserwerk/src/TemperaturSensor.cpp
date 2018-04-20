/*
 * TemperaturSensor.cpp
 *
 *  Created on: 08.04.2018
 *      Author: Jan-Gerd
 */

#include "TemperaturSensor.h"
#include <dirent.h>
#include <cstdint>
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Stoppuhr.h"

TemperaturSensor::TemperaturSensor() :
		hole_temperatur_thread(&TemperaturSensor::hole_temperatur, this),

		temperatur_holen(true)
{
}

TemperaturSensor::~TemperaturSensor()
{
	temperatur_holen = false;

	hole_temperatur_thread.join();
}

float TemperaturSensor::gibTemperatur(void)
{
	temperatur_mutex.lock();
	float retval = 0.0;

	if (temperatur.size())
	{
		retval = temperatur.back().second;
		temperatur_mutex.unlock();
	}

	return retval;
}

void TemperaturSensor::hole_temperatur(TemperaturSensor* ts)
{
	DIR *dir;
	struct dirent *dirent;
	char dev[16];      // Dev ID
	char path[] = "/sys/bus/w1/devices";

	dir = opendir(path);
	if (dir != NULL)
	{
		while ((dirent = readdir(dir)))
			// 1-wire devices are links beginning with 28-
			if (dirent->d_type == DT_LNK
					&& strstr(dirent->d_name, "28-") != NULL)
			{
				strcpy(dev, dirent->d_name);
				cout << endl << "Device: " << dev << endl;
			}
		(void) closedir(dir);
	}
	else
	{
		cerr << "Couldn't open the w1 devices directory" << endl;
		;
	}

	// Assemble path to OneWire device
	string sensorpfad = path;
	sensorpfad += "/";
	sensorpfad += dev;
	sensorpfad += "/w1_slave";

	while (ts->temperatur_holen)
	{
		string zeile;
		ifstream myfile(sensorpfad);
		if (myfile.is_open())
		{
			while (getline(myfile, zeile))
			{
				size_t position = zeile.find("t=");
				if (position != string::npos)
				{
					string temperatur_string = zeile.substr(position + 2);
					ts->temperatur_mutex.lock();
					float aktuelle_temperatur = float(
							stoi(temperatur_string, NULL, 10)) / 1000.0;
					int d = 2;
					if (ts->temperatur.size())
					{
						aktuelle_temperatur = (d * aktuelle_temperatur
								+ ts->temperatur.front().second) / (1 + d);
					}

					ts->temperatur.emplace_back(Stoppuhr::gibZeit(),
							aktuelle_temperatur);
					if (ts->temperatur.size() > 100000)
					{
						ts->temperatur.pop_front();
					}
					ts->temperatur_mutex.unlock();
				}
			}

			myfile.close();
		}
		else
		{
			cout << "Unable to open file" << endl;
		}
	}
}
