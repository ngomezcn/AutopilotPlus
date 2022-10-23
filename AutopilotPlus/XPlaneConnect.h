
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
		unrequest_datarefs();
		request_datarefs();
		this->init_udp_service();
	}

	unsigned char* build_rref_msg(DREF* dref)
	{
		const unsigned char HEADER[] = "RREF"; // TODO: Se podria crear algun enum, struct.... para gestionar los headers en el namespace xplane 

		unsigned char* data = reinterpret_cast<unsigned char*>(&(*dref).dref_request);
		unsigned char msg[sizeof(HEADER) + sizeof(DREF_REQUEST)];

		memcpy(msg, HEADER, sizeof(HEADER));
		memcpy(msg + sizeof(HEADER), data, sizeof(DREF_REQUEST));

		return msg;
	}

	void unrequest_datarefs()
	{
		for (DREF* dataref : drefs_request_list)
		{
			int tmp_dref_freq_ = dataref->dref_request.dref_freq_;
			dataref->dref_request.dref_freq_ = xplane::DREF_PERIOD::PERIOD_NONE;

			Log::debug("DREF unrequested: Path:[{0}], Freq:{1}, Index:{2}", dataref->dref_request.dref_string_, dataref->dref_request.dref_freq_, dataref->dref_request.dref_sender_index_);

			/*unsigned char* data = reinterpret_cast<unsigned char*>(&(*dataref).dref_request);
			unsigned char header[] = "RREF";
			unsigned char msg[sizeof(header) + sizeof(DREF_REQUEST)];

			memcpy(msg, header, sizeof(header));
			memcpy(msg + sizeof(header), data, sizeof(DREF_REQUEST));*/

			auto msg = build_rref_msg(dataref);

			dataref->dref_request.dref_freq_ = tmp_dref_freq_;
			this->send(msg, sizeof(msg));
		}
	}

	void request_datarefs()
	{
		for (DREF* dataref : drefs_request_list)
		{
			if (dataref->dref_request.dref_freq_ > xplane::DREF_PERIOD::PERIOD_NONE)
				Log::debug("DREF requested: Path:[{0}], Freq:{1}, Index:{2}", dataref->dref_request.dref_string_, dataref->dref_request.dref_freq_, dataref->dref_request.dref_sender_index_);
			else
				Log::warning("DREF requested: Path:[{0}], Freq:{1}, Index:{2} (warn: FREQ = 0)", dataref->dref_request.dref_string_, dataref->dref_request.dref_freq_, dataref->dref_request.dref_sender_index_);

			drefs_map.insert(std::make_pair((*dataref).dref_request.dref_sender_index_, dataref));

			auto msg = build_rref_msg(dataref);

			this->send(msg, sizeof(msg));
		}
		Log::info("Total DREFs requested: {0}", drefs_request_list.size());
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
							//TODO: Poner mas validaciones para evitar Invalid Memory Access Violation, ya que no se puede gestionar por excepciones
							(*drefs_map[dref_in.sender_index]).value_ = dref_in.flt_value;
						}
						catch (std::exception& e)
						{
							Log::error("Ha ocurrido un error al procesar los datos recibidos. INFO: {0}, {1}", e.what(), header);
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
		else
		{
			throw std::runtime_error(std::string("handle_receive(): ") + error.message());
		}
	}

	~XPlaneConnect()
	{
		unrequest_datarefs();
	}
};

#endif /* XPLANECONNECTOR_H */