#include <iostream>

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
	try
	{
		initial_datarefs_request();
		udp_service.init_service();
		

		while (true){
			local_y.set(500);

			//std::cout << elevation.get() << " - " << latitude.get() << " - " << longitude.get() << '\r';
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
    return 0;
}