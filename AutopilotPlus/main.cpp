#include <iostream>
#include <map>
#include <boost/system/error_code.hpp>

#include "XPlaneConnect.h"

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

	~Core()
	{
		delete(xplane_connect);
	}
	void init()
	{
		auto system_ip = "192.168.8.101";
		auto xplane_port = "49000";
		auto local_port = "17";

		try
		{
			this->setup_logger();
			xplane_connect = new XPlaneConnect(system_ip, xplane_port, local_port);
			xplane_connect->init();

			while (true) {
				local_y.set(500);
				std::cout << elevation.get() << " - " << latitude.get() << " - " << longitude.get() << '\r';
			}
		}
		catch (std::exception& e)
		{
			LOG_ERROR(e.what());
		}
	}

	void setup_logger() const
	{
		const char* log_pattern = "[%t] %^[%l]%$ %v";
		spdlog::set_pattern(log_pattern);
		spdlog::set_level(LOG_LEVEL);

		LOG_INFO("Setting logger => Level:{0}, Pattern: '{1}'", LOG_LEVEL, log_pattern);
	}
};

int DREF::serial_id = 0;
int main()
{
	Core o_core;
	o_core.init();

	//TODO : Al clicar para cerrar la ventana asegurarse de enviar FREQ = 0 a todas las variables para asi evitar errores https://stackoverflow.com/questions/21404256/call-function-right-before-termination-c
}