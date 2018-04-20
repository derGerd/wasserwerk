//============================================================================
// Name        : wasserwerk.cpp
// Author      : Jan-Gerd Meinen
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

/* Copyright (c) 2013-2018 the Civetweb developers
 * Copyright (c) 2013 No Face Press, LLC
 * License http://opensource.org/licenses/mit-license.php MIT License
 */

// Simple example program on how to use Embedded C++ interface.
#include "include/CivetServer.h"
#include <cstring>
#include "TemperaturSensor.h"
#include "WebStartseite.h"
#include "Durchflusssensor.h"
#include "MotorSchieber.h"
#include <iostream>
#include <fstream>
#include <unistd.h>

#define DOCUMENT_ROOT "."
#define PORT "8081"
#define WASSERWERK_URI "/"

bool exitNow = false;

class ExitHandler: public CivetHandler
{
public:
	bool handleGet(CivetServer *server, struct mg_connection *conn)
	{
		mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: "
				"text/plain\r\nConnection: close\r\n\r\n");
		mg_printf(conn, "Bye!\n");
		exitNow = true;
		return true;
	}
};

class AHandler: public CivetHandler
{
	MotorSchieber schieber;
private:
	bool handleAll(const char *method, CivetServer *server,
			struct mg_connection *conn)
	{
		std::string s = "";
		mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: "
				"text/html\r\nConnection: close\r\n\r\n");
		mg_printf(conn, "<html><body>");
		mg_printf(conn, "<h2>This is the A handler for \"%s\" !</h2>", method);

		if (CivetServer::getParam(conn, "param", s))
		{
			if (s == "1")
			{
				schieber.schieber_auf();
			}
			else if (s == "2")
			{
				schieber.schieber_zu();
			}
			else if (s == "3")
			{
				schieber.schieber_stopp();
			}
		}
		mg_printf(conn,
				"<p>Schieber auf <a href=\"a?param=1\">click here</a></p>\r\n");

		mg_printf(conn,
				"<p>Schieber zu <a href=\"a?param=2\">click here</a></p>\r\n");

		mg_printf(conn,
				"<p>Schieber stopp <a href=\"a?param=3\">click here</a></p>\r\n");

		mg_printf(conn, "</body></html>\n");
		return true;
	}

public:
	bool handleGet(CivetServer *server, struct mg_connection *conn)
	{
		return handleAll("GET", server, conn);
	}
	bool handlePost(CivetServer *server, struct mg_connection *conn)
	{
		return handleAll("POST", server, conn);
	}
};

class DateiHandler: public CivetHandler
{
private:

	string pfad;

	bool handleAll(const char *method, CivetServer *server,
			struct mg_connection *conn)
	{
		ifstream myfile(pfad);
		if (myfile.is_open())
		{
			string zeile;
			while (getline(myfile, zeile))
			{
				zeile += "\r\n";
				mg_write(conn, zeile.c_str(), zeile.size());
			}
		}
		return true;
	}

public:
	DateiHandler(string _pfad) :
			pfad(_pfad)
	{

	}
	bool handleGet(CivetServer *server, struct mg_connection *conn)
	{
		return handleAll("GET", server, conn);
	}
};

class BinaerDateiHandler: public CivetHandler
{
private:

	string pfad;

	bool handleAll(const char *method, CivetServer *server,
			struct mg_connection *conn)
	{

		vector<char> daten;
		ifstream file(pfad, ios_base::binary);
		daten.assign(istreambuf_iterator<char>(file),
				istreambuf_iterator<char>());

		mg_write(conn, daten.data(), daten.size());
		return true;
	}

public:
	BinaerDateiHandler(string _pfad) :
			pfad(_pfad)
	{

	}
	bool handleGet(CivetServer *server, struct mg_connection *conn)
	{
		return handleAll("GET", server, conn);
	}
};

class ABHandler: public CivetHandler
{
public:
	bool handleGet(CivetServer *server, struct mg_connection *conn)
	{
		mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: "
				"text/html\r\nConnection: close\r\n\r\n");
		mg_printf(conn, "<html><body>");
		mg_printf(conn, "<h2>This is the AB handler!!!</h2>");
		mg_printf(conn, "</body></html>\n");
		return true;
	}
};

class FooHandler: public CivetHandler
{
public:
	bool handleGet(CivetServer *server, struct mg_connection *conn)
	{
		/* Handler may access the request info using mg_get_request_info */
		const struct mg_request_info *req_info = mg_get_request_info(conn);

		mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: "
				"text/html\r\nConnection: close\r\n\r\n");

		mg_printf(conn, "<html><body>\n");
		mg_printf(conn, "<h2>This is the Foo GET handler!!!</h2>\n");
		mg_printf(conn, "<p>The request was:<br><pre>%s %s HTTP/%s</pre></p>\n",
				req_info->request_method, req_info->request_uri,
				req_info->http_version);
		mg_printf(conn, "</body></html>\n");

		return true;
	}
	bool handlePost(CivetServer *server, struct mg_connection *conn)
	{
		/* Handler may access the request info using mg_get_request_info */
		const struct mg_request_info *req_info = mg_get_request_info(conn);
		long long rlen, wlen;
		long long nlen = 0;
		long long tlen = req_info->content_length;
		char buf[1024];

		mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: "
				"text/html\r\nConnection: close\r\n\r\n");

		mg_printf(conn, "<html><body>\n");
		mg_printf(conn, "<h2>This is the Foo POST handler!!!</h2>\n");
		mg_printf(conn, "<p>The request was:<br><pre>%s %s HTTP/%s</pre></p>\n",
				req_info->request_method, req_info->request_uri,
				req_info->http_version);
		mg_printf(conn, "<p>Content Length: %li</p>\n", (long) tlen);
		mg_printf(conn, "<pre>\n");

		while (nlen < tlen)
		{
			rlen = tlen - nlen;
			if (rlen > sizeof(buf))
			{
				rlen = sizeof(buf);
			}
			rlen = mg_read(conn, buf, (size_t) rlen);
			if (rlen <= 0)
			{
				break;
			}
			wlen = mg_write(conn, buf, (size_t) rlen);
			if (wlen != rlen)
			{
				break;
			}
			nlen += wlen;
		}

		mg_printf(conn, "\n</pre>\n");
		mg_printf(conn, "</body></html>\n");

		return true;
	}
};

int main(int argc, char *argv[])
{
	const char *options[] =
	{ "document_root", DOCUMENT_ROOT, "listening_ports", PORT, 0 };

	std::vector<std::string> cpp_options;
	for (unsigned int i = 0; i < (sizeof(options) / sizeof(options[0]) - 1);
			i++)
	{
		cpp_options.push_back(options[i]);
	}

	TemperaturSensor aussentemperaturSensor;

	// CivetServer server(options); // <-- C style start
	CivetServer server(cpp_options); // <-- C++ style start

	WebStartseite wasserwerkStart(aussentemperaturSensor);
	server.addHandler(WASSERWERK_URI, wasserwerkStart);

	ExitHandler h_exit;
	server.addHandler(EXIT_URI, h_exit);

	AHandler h_a;
	server.addHandler("/a", h_a);

	DateiHandler h_script_js("/home/pi/wasserwerk/script.js");
	server.addHandler("/wasserwerk/script.js", h_script_js);

	DateiHandler h_style_css("/home/pi/wasserwerk/style.css");
	server.addHandler("/wasserwerk/style.css", h_style_css);

	BinaerDateiHandler h_wasserhahn_ico("/home/pi/wasserwerk/wasserhahn.ico");
	server.addHandler("/wasserwerk/wasserhahn.ico", h_wasserhahn_ico);

	ABHandler h_ab;
	server.addHandler("/a/b", h_ab);

	FooHandler h_foo;
	server.addHandler("**.foo", h_foo);
	printf("Browse files at http://localhost:%s/\n", PORT);

	printf("Run example at http://localhost:%s%s\n", PORT, WASSERWERK_URI);
	printf("Exit at http://localhost:%s%s\n", PORT, EXIT_URI);

	if (gpioInitialise() > 0)
	{
		Durchflusssensor durchflusssensor1;
	}
	else
	{
		cerr << "gpio konnte nicht initialisiert werden!" << endl;
	}

	while (!exitNow)
	{
		sleep(1);
	}
	gpioTerminate();
	printf("Bye!\n");

	return 0;
}
