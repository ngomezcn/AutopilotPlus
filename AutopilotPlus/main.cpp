#include <iostream>

#include "logger.h"
#include "UDPService.h"
#include "DREF_REQUEST.h"
#include "DREFs_definition.h"

using namespace boost;
using namespace xplane::sim::flightmodel::position;

class xplane_connector
{
	UDPService* xplane_service;
	std::vector<DREF*> drefs_request_list{
			&local_y,
			&local_z,
			&lat_ref,
			&lon_ref,
			&elevation,
			&latitude,
			&longitude
	};
	DREF_INPUT dref_in;
};

class Core {

	UDPService* xplane_service;
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

public:
	Core() {
		this->setup_logger();

		const auto system_ip = "192.168.8.101";
		const auto xplane_port = "49000";
		const auto local_port = "192.168.8.101";
		xplane_service = new UDPService(system_ip, xplane_port, local_port, drefs_map);
		this->datarefs_request();
	}

	void init()
	{
		try
		{
			this->xplane_service->init_service();

			while (true) {
				local_y.set(500);
				std::cout << elevation.get() << " - " << latitude.get() << " - " << longitude.get() << "\r";
			}
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void datarefs_request()
	{
		for (DREF* dataref : drefs_request_list)
		{
			if (dataref->dref_req.dref_freq_ > 0)
				LOG_INFO("DATAREF requested: Path:[{0}], Freq:{1}, Index:{2}", dataref->dref_req.dref_string_, dataref->dref_req.dref_freq_, dataref->dref_req.dref_sender_index_);
			else
				LOG_WARNING("DATAREF requested: Path:[{0}], Freq:{1}, Index:{2} (warn: FREQ = 0)", dataref->dref_req.dref_string_, dataref->dref_req.dref_freq_, dataref->dref_req.dref_sender_index_);

			drefs_map.insert(std::make_pair((*dataref).id, dataref));

			unsigned char* data = reinterpret_cast<unsigned char*>(&(*dataref).dref_req);
			unsigned char header[] = "RREF";
			unsigned char msg[sizeof(header) + sizeof(DREF_REQUEST)];

			memcpy(msg, header, sizeof(header));
			memcpy(msg + sizeof(header), data, sizeof(DREF_REQUEST));

			this->xplane_service->send(msg, sizeof(msg));
		}
		LOG_INFO("Total DATAREFs requested: {0}", drefs_request_list.size());
	}

	~Core() {
		LOG_DEBUG("Core: destructor");
		delete(xplane_service);
	}

	void setup_logger() const
	{
		const char* log_pattern = "[%t] %^[%l]%$ %v";
		spdlog::set_pattern(log_pattern);
		spdlog::set_level(LOG_LEVEL);

		LOG_INFO("Setting logger => Level:{0}, Pattern: '{1}'", LOG_LEVEL, log_pattern);
	}
};

int main()
{
	Core o_core;
	o_core.init();
}