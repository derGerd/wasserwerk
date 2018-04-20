/*
 * WebStartseite.cpp
 *
 *  Created on: 08.04.2018
 *      Author: Jan-Gerd
 */

#include "WebStartseite.h"
#include <sstream>
#include <iomanip>
#include "Stoppuhr.h"

WebStartseite::WebStartseite(TemperaturSensor & _ts) :
		ts(_ts)
{
	// TODO Auto-generated constructor stub

}

WebStartseite::~WebStartseite()
{
	// TODO Auto-generated destructor stub
}

bool WebStartseite::handleGet(CivetServer *server, struct mg_connection *conn)
{
	stringstream webseite("");

	webseite
			<< "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n";

	webseite << "<html>";
	webseite << "<head>\r\n";
	webseite << "<title>Wasserwerk</title>\r\n";
	webseite << "<link rel=\"icon\" href=\"wasserwerk/wasserhahn.ico\" type=\"image/x-icon\">" << endl;
//	webseite << "<script src=\"../node_modules/chart.js\"></script>\r\n";
	webseite
			<< "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.3.0/Chart.min.js\"></script>"
			<< endl;
	webseite
			<< "<link rel=\"stylesheet\" type=\"text/css\" href=\"wasserwerk/style.css\">\r\n";
	webseite << "</head>" << endl;

	webseite << "<body>\r\n";

	webseite << "<div class=\"wrapper\">" << endl;
	webseite << "<h1>Wasserwerk</h1>" << endl;
	webseite << "<h2>Temperatur</h2>" << endl;

	webseite << "<canvas id=\"myChart\" width=\"5000\" height=\"2500\"></canvas>"
			<< endl;
	webseite << "</div>" << endl;

	webseite << "<script>" << endl;

	webseite << "var zeit = [";

	int i = 0;

	ts.temperatur_mutex.lock();

	int xBeschriftungTeiler = ts.temperatur.size() / 50;

	for (auto temperatur : ts.temperatur)
	{
		if (i++ == xBeschriftungTeiler)
		{
			i = 0;
			webseite << "'" << temperatur.first << "'";
			webseite << ",";
		}
	}

	if (ts.temperatur.size())
	{
		webseite << "'" << ts.temperatur.back().first << "'";
	}

	webseite << "];" << endl;
	i = 0;
	webseite << "var temperatur = [";

	for (auto temperatur : ts.temperatur)
	{
		if (i++ == xBeschriftungTeiler)
		{
			i = 0;
			webseite << /*setprecision(2) <<*/temperatur.second;
			webseite << ",";
		}
	}

	if (ts.temperatur.size())
	{
		webseite << "'" << ts.temperatur.back().second << "'";
	}

	ts.temperatur_mutex.unlock();

	webseite << "];" << endl;

	webseite << "</script>" << endl;

	webseite << "<script src=\"wasserwerk/script.js\"></script>" << endl;

	webseite << "<h2>This is an example text from a C++ handler</h2>\r\n";

	webseite << "Die aktuelle Au&szligentemperatur ist: ";

	webseite << ts.gibTemperatur() << "\r\n";

	webseite << "<p>To see a page from the A handler <a "
			"href=\"a\">click here</a></p>\r\n";
	webseite << "<p>To see a page from the A handler with a parameter "
			"<a href=\"a?param=1\">click here</a></p>\r\n";
	webseite << "<p>To see a page from the A/B handler <a "
			"href=\"a/b\">click here</a></p>\r\n";
	webseite << "<p>To see a page from the *.foo handler <a "
			"href=\"xy.foo\">click here</a></p>\r\n";
	webseite << "<p>To see a page from the WebSocket handler <a "
			"href=\"ws\">click here</a></p>\r\n";
	webseite << "<p>To exit <a href=\"";
	webseite << EXIT_URI
	;
	webseite << "\">click here</a></p>\r\n";
	webseite << "</body></html>\r\n";

	mg_write(conn, webseite.str().c_str(), webseite.str().size());
	return true;
}
