#include <iostream>


#include "autopilotplus.cpp"
#include "spdlog/spdlog.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#define LOG_TRACE(...) spdlog::trace(__VA_ARGS__);
#define LOG_DEBUG(...) spdlog::debug(__VA_ARGS__);
#define LOG_INFO(...)  spdlog::info(__VA_ARGS__);
#define LOG_ERROR(...) spdlog::error(__VA_ARGS__);

#include "UDPService.h"

#include "DREF_REQUEST.h"
#include "datarefs_loader.h"

using namespace boost;

void initial_datarefs_request() {
	for (DREF* dataref : DREFs_TO_REQUEST)
	{
		DATAREFS_MAP.insert(std::make_pair((*dataref).id, dataref));

		unsigned char* data = reinterpret_cast<unsigned char*>(&(*dataref).dref_req);
		unsigned char header[] = "RREF";
		unsigned char msg[sizeof(header) + sizeof(DREF_REQUEST)];

		memcpy(msg, header, sizeof(header));
		memcpy(msg + sizeof(header), data, sizeof(DREF_REQUEST));

		udp_service.send(msg, sizeof(msg));
	}
}
using namespace sim::flightmodel::position;


int main()
{
	spdlog::set_pattern("[%t] %^[%l]%$ %v");

	LOG_DEBUG("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
	//// [2022-10-02 23:51:48.948] [info] Welcome to spdlog!

	//spdlog::info("Welcome to spdlog!");
	//spdlog::error("Some error message with arg: {}", 1);

	//spdlog::warn("Easy padding in numbers like {:08d}", 12);
	//spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
	//spdlog::info("Support for floats {:03.2f}", 1.23456);
	//spdlog::info("Positional args are {1} {0}..", "too", "supported");
	//spdlog::trace("{:<30}", "left aligned");


	//// change log pattern
	//spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

	//// Compile time log levels
	//// define SPDLOG_ACTIVE_LEVEL to desired level
	//SPDLOG_TRACE("Some trace message with param {}", 42);
	SPDLOG_DEBUG("Some debug message");

	try
	{
		initial_datarefs_request();
		udp_service.init_service();

		while (true){
			//local_y.set(500);
			//std::cout << elevation.get() << " - " << latitude.get() << " - " << longitude.get() << '\r';
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
    return 0;
}