#include <iostream>
#include <map>
#include <stdio.h>

#include <boost/system/error_code.hpp>
#include "XPlaneConnect.h"

#include <windows.h> // Always put windows.h after asio.hpp to avoid compatibility-issues

using namespace boost;
using namespace xplane::sim::flightmodel::position;

class Core {

	//UDPService* xplane_service;
	std::vector<DREF*> drefs_request_list{
			&local_y,
			&local_z,
			&lat_ref,
			&lon_ref,
			&elevation,
			&latitude,
			&longitude
	};
	std::map<int, DREF*> drefs_map;
	XPlaneConnect* xplane_connect;

public:

	void init()
	{
		auto system_ip = "192.168.8.101";
		auto xplane_port = "49000";
		auto local_port = "17";

		try
		{
			Log::setSettings();

			xplane_connect = new XPlaneConnect(system_ip, xplane_port, local_port);
			xplane_connect->init();

			while (true) {
				local_y.set(500);
				std::cout << elevation.get() << " - " << latitude.get() << " - " << longitude.get() << '\r';
			}
		}
		catch (std::exception& e)
		{
			Log::error(e.what());
		}
	}

	~Core()
	{
		delete(xplane_connect);
	}
};



int DREF::serial_id = 0;
int main()
{
	/*HWND hwnd = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(hwnd, FALSE);
	EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);*/

	Core o_core;
	o_core.init();
}