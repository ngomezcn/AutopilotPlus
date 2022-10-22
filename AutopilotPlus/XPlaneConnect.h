
#ifndef XPLANECONNECT_H
#define XPLANECONNECT_H

#include <vector>

#include "UDPService.h"

class XPlaneConnect : UDPService
{
public:
	DREF_INPUT dref_in;
	std::map<int, DREF*> drefs_map;

	std::vector<DREF*> drefs_request_list{
		&xplane::sim::flightmodel::position::local_y,
		&xplane::sim::flightmodel::position::local_z,
		&xplane::sim::flightmodel::position::lat_ref,
		&xplane::sim::flightmodel::position::lon_ref,
		&xplane::sim::flightmodel::position::elevation,
		&xplane::sim::flightmodel::position::latitude,
		&xplane::sim::flightmodel::position::longitude
	};

	XPlaneConnect(const char* system_ip, const char* xplane_port, const char* local_port) : UDPService(system_ip, xplane_port, local_port)
	{
	}

	void init()
	{
		request_datarefs();
		this->init_udp_service();
	}

	void request_datarefs()
	{
		for (DREF* dataref : drefs_request_list)
		{
			if (dataref->dref_req.dref_freq_ > xplane::DREF_PERIOD::PERIOD_NONE)
				LOG_DEBUG("DREF requested: Path:[{0}], Freq:{1}, Index:{2}", dataref->dref_req.dref_string_, dataref->dref_req.dref_freq_, dataref->dref_req.dref_sender_index_);
			else
				LOG_WARNING("DREF requested: Path:[{0}], Freq:{1}, Index:{2} (warn: FREQ = 0)", dataref->dref_req.dref_string_, dataref->dref_req.dref_freq_, dataref->dref_req.dref_sender_index_);

			drefs_map.insert(std::make_pair((*dataref).id, dataref));

			unsigned char* data = reinterpret_cast<unsigned char*>(&(*dataref).dref_req);
			unsigned char header[] = "RREF";
			unsigned char msg[sizeof(header) + sizeof(DREF_REQUEST)];

			memcpy(msg, header, sizeof(header));
			memcpy(msg + sizeof(header), data, sizeof(DREF_REQUEST));

			this->send(msg, sizeof(msg));
		}
		LOG_INFO("Total DREFs requested: {0}", drefs_request_list.size());
	}

	void dispatch_received_data(char header[6], std::size_t size_in_bytes)
	{
		if (strcmp("RREF,", header) == 0)
		{
			
				constexpr int header_size = 5;
				int dref_in_filled_bytes = 1;
				int cuadrar = 0;
				const auto data = reinterpret_cast<unsigned char*>(&dref_in);

				for (size_t i = header_size; i < size_in_bytes; i++)
				{
					data[i - header_size - cuadrar] = this->received_buffer[i];

					if (dref_in_filled_bytes == 8) {
						try
						{
							(*drefs_map[dref_in.sender_index]).value_ = dref_in.flt_value;
						}
						catch (std::exception& e)
						{
							LOG_ERROR("Ha ocurrido un error al procesar los datos recibidos. INFO: {0}", header);
						}
						dref_in_filled_bytes = 0;
						cuadrar += 8;
					}
					dref_in_filled_bytes++;
				}
		}

		//TODO: Hace esto mas de manera que acepte otro tipo de headers.
		const char a[] = "RREF,";
		
	}

	void handle_receive(const boost::system::error_code& error, std::size_t size_in_bytes) override
	{
		if (!error || error == boost::asio::error::message_size)
		{
			char header[6] = "";
			for (size_t i = 0; i < 5; i++)
				header[i] = received_buffer[i];
			dispatch_received_data(header, size_in_bytes);
			
			start_async_receiver();
		}
		else { LOG_ERROR("handle_receive(): {}", error.message()); }
	}
};

#endif /* XPLANECONNECTOR_H */