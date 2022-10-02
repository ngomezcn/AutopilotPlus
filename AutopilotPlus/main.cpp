#include <iostream>
#include "UDPService.h"
#include "DREF_REQ.h"
#include "sim.h"

using namespace boost;

int main()
{
	try
	{
		boost::asio::io_service io_service;
		UDPService udp_service(
			io_service,
			"192.168.8.103", "49000",
			15);


		for (simvar* x : DATAREFS_TO_LOAD)
		{
			DATAREFS_MAP.insert(std::make_pair((*x).id, x));

			unsigned char* data = reinterpret_cast<unsigned char*>(&(*x).rref);
			unsigned char header[] = "RREF";
			unsigned char msg[sizeof(header) + sizeof(DREF_REQ)];

			memcpy(msg, header, sizeof(header));
			memcpy(msg + sizeof(header), data, sizeof(DREF_REQ));

			udp_service.send(msg, sizeof(msg));
		}

		while (true)
		{
			io_service.run();
			std::cout << sim::flightmodel::position::latitude.get() << std::endl;
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
    return 0;
}