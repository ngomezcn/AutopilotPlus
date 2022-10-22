
#ifndef XPLANECONNECT_H
#define XPLANECONNECT_H

#include <vector>

#include "UDPService.h"

class XPlaneConnect : UDPService
{
public:
	DREF_INPUT dref_in;
	std::map<int, DREF*> drefs_map;

	void datarefs_request()
	{
		for (DREF* dataref : drefs_request_list)
		{
			if (dataref->dref_req.dref_freq_ > 0)
				LOG_DEBUG("DATAREF requested: Path:[{0}], Freq:{1}, Index:{2}", dataref->dref_req.dref_string_, dataref->dref_req.dref_freq_, dataref->dref_req.dref_sender_index_);
			else
				LOG_WARNING("DATAREF requested: Path:[{0}], Freq:{1}, Index:{2} (warn: FREQ = 0)", dataref->dref_req.dref_string_, dataref->dref_req.dref_freq_, dataref->dref_req.dref_sender_index_);

			drefs_map.insert(std::make_pair((*dataref).id, dataref));

			unsigned char* data = reinterpret_cast<unsigned char*>(&(*dataref).dref_req);
			unsigned char header[] = "RREF";
			unsigned char msg[sizeof(header) + sizeof(DREF_REQUEST)];

			memcpy(msg, header, sizeof(header));
			memcpy(msg + sizeof(header), data, sizeof(DREF_REQUEST));

			this->send(msg, sizeof(msg));
		}
		LOG_INFO("Total DATAREFs requested: {0}", drefs_request_list.size());
	}

	XPlaneConnect(const char* system_ip, const char* xplane_port, const char* local_port) : UDPService(system_ip, xplane_port, local_port)
	{
		std::map<int, int>* mapA;
		datarefs_request();
		this->init_service();

		try
		{
			while (true) {
				xplane::sim::flightmodel::position::local_y.set(500);
				std::cout << xplane::sim::flightmodel::position::elevation.get() << " - " << xplane::sim::flightmodel::position::latitude.get() << " - " << xplane::sim::flightmodel::position::longitude.get() << "\r";
			}
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	std::vector<DREF*> drefs_request_list{
			&xplane::sim::flightmodel::position::local_y,
			&xplane::sim::flightmodel::position::local_z,
			&xplane::sim::flightmodel::position::lat_ref,
			&xplane::sim::flightmodel::position::lon_ref,
			&xplane::sim::flightmodel::position::elevation,
			&xplane::sim::flightmodel::position::latitude,
			&xplane::sim::flightmodel::position::longitude
	};

	void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred)
	{
		if (!error || error == boost::asio::error::message_size)
		{
			char header[6] = "";
			for (size_t i = 0; i < 5; i++)
				header[i] = recv_buffer_[i];

			//TODO: Hace esto mas de manera que acepte otro tipo de headers.
			const char a[] = "RREF,";
			if (strcmp(a, header) == 0)
			{
				constexpr int header_size = 5;
				int dref_in_filled_bytes = 1;
				int cuadrar = 0;
				const auto data = reinterpret_cast<unsigned char*>(&dref_in);

				for (size_t i = header_size; i < bytes_transferred; i++)
				{
					data[i - header_size - cuadrar] = recv_buffer_[i];

					if (dref_in_filled_bytes == 8) {
						try
						{
							(*drefs_map[dref_in.sender_index]).value_ = dref_in.flt_value;
						}
						catch (const std::exception&)
						{
							std::cout << "No se ha podido actualizar la simvar" << std::endl;
							throw;
						}
						dref_in_filled_bytes = 0;
						cuadrar += 8;
					}
					dref_in_filled_bytes++;
				}
			}
			start_async_receiver();
		}
		else { LOG_ERROR("handle_receive(): {}", error.message()); }
	}
};

#endif /* XPLANECONNECTOR_H */