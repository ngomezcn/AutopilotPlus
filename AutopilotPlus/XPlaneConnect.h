
#ifndef XPLANECONNECT_H
#define XPLANECONNECT_H

#include <vector>

#include "UDPService.h"

class XPlaneConnect : UDPService
{
public:
	DREF_INPUT dref_in;
	std::map<int, DREF*> MAP_REQUESTED_DREFs;

	const std::vector<DREF*> DREFs_REQUEST_LIST {
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
		unrequest_DREFs_list(); // Nos aseguramos de que xplane no este enviando datos antes de empezar a recibir los datos
		request_DREFs_list();
		this->start_udp_receiver();
	}

	void unrequest_DREFs_list();

	void request_DREFs_list();

	/**
	 * @bug: Devuelve el puntero de una variable local, hay que hacer que pase el return por referencia en los parametros
	 */
	static unsigned char* build_rref_msg(DREF* dref);

	/**
	 * @details Es llamada como callback desde el hilo de servidor UDP cada vez que se recibe informacion
	 * @code  socket_.async_receive_from(
	 *		boost::asio::buffer(received_buffer),
	 *		remote_endpoint_,
	 *		boost::bind(&UDPService::handle_receive, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
	 * @endcode
	 */
	void handle_receive(const boost::system::error_code& error, std::size_t size_in_bytes) override;

	/**
	 * @details Actualiza los DREFs previamente solicitados segun el header del los datos recibidos
	 */
	void dispatch_received_data(char header[6], std::size_t size_in_bytes);

	~XPlaneConnect();
};

inline void XPlaneConnect::unrequest_DREFs_list()
{
	for (DREF* dataref : DREFs_REQUEST_LIST)
	{
		int saved_frequency = dataref->dref_request.dref_frequency;
		dataref->dref_request.dref_frequency = xplane::DREF_PERIOD::PERIOD_NONE;

		auto msg = build_rref_msg(dataref);

		dataref->dref_request.dref_frequency = saved_frequency;
		this->send(msg, sizeof(msg));

		Log::debug("DREF unrequested: Path:[{0}], Freq:{1}, Index:{2}", dataref->dref_request.dref_string_, dataref->dref_request.dref_frequency, dataref->dref_request.dref_sender_index_);
	}
}

inline void XPlaneConnect::request_DREFs_list()
{

	for (DREF* dataref : DREFs_REQUEST_LIST)
	{
		auto msg = build_rref_msg(dataref);
		this->send(msg, sizeof(msg));
		MAP_REQUESTED_DREFs.insert(std::make_pair((*dataref).dref_request.dref_sender_index_, dataref));

		// Registering a warning if we request a datarefs with sending frequency of 0 
		if (dataref->dref_request.dref_frequency > xplane::DREF_PERIOD::PERIOD_NONE) { Log::debug("DREF requested: Path:[{0}], Freq:{1}, Index:{2}", dataref->dref_request.dref_string_, dataref->dref_request.dref_frequency, dataref->dref_request.dref_sender_index_); }
		else { Log::warning("DREF requested: Path:[{0}], Freq:{1}, Index:{2} (warn: FREQ = 0)", dataref->dref_request.dref_string_, dataref->dref_request.dref_frequency, dataref->dref_request.dref_sender_index_); }
	}
	Log::info("Total DREFs requested: {0}", DREFs_REQUEST_LIST.size());
}

inline void XPlaneConnect::dispatch_received_data(char header[6], std::size_t size_in_bytes)
{
	if (strcmp("RREF,", header) == 0)
	{
		constexpr int header_size = 5;
		int dref_in_filled_bytes = 1;
		int cuadrar = 0;
		const auto data = reinterpret_cast<unsigned char*>(&dref_in);

		// TODO: Hacer mas legible este algoritmo
		for (size_t i = header_size; i < size_in_bytes; i++)
		{
			data[i - header_size - cuadrar] = this->received_buffer[i];

			if (dref_in_filled_bytes == 8) {
				try
				{
					//TODO: Poner mas validaciones para evitar Invalid Memory Access Violation, ya que no se puede gestionar por excepciones al se un ex de OS
					(*MAP_REQUESTED_DREFs[dref_in.sender_index]).value_ = dref_in.flt_value;
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
}

inline void XPlaneConnect::handle_receive(const boost::system::error_code& error, std::size_t size_recv_data)
{
	if (!error || error == boost::asio::error::message_size)
	{
		char header[6] = "";
		for (size_t i = 0; i < 5; i++)
			header[i] = received_buffer[i];

		dispatch_received_data(header, size_recv_data);
		async_receive(); 
	}
	else
	{
		throw std::runtime_error(std::string("handle_receive(): ") + error.message());
	}
}

inline unsigned char* XPlaneConnect::build_rref_msg(DREF* dref)
{
	const unsigned char HEADER[] = "RREF"; // TODO: Se podria crear algun enum, struct.... para gestionar los headers en el namespace xplane 

	unsigned char* data = reinterpret_cast<unsigned char*>(&(*dref).dref_request);
	unsigned char msg[sizeof(HEADER) + sizeof(DREF_REQUEST)];

	memcpy(msg, HEADER, sizeof(HEADER));
	memcpy(msg + sizeof(HEADER), data, sizeof(DREF_REQUEST));

	return msg;
}

inline XPlaneConnect::~XPlaneConnect()
{
	unrequest_DREFs_list();
}

#endif /* XPLANECONNECTOR_H */
