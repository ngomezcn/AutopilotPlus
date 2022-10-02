#ifndef UDPSERVER_H
#define UDPSERVER_H

#define NULL_TERMINATE_SAFELY 1

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <iomanip>
#include <sstream>
#include "xplane_types.h"

#include "DREF_INPUT.h"
#include "datarefs_map.h"

using namespace boost;
using namespace boost::asio::ip;

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

class UDPService
{
public:
	
	UDPService(
		const char* host,
		const char* cport,
		int sport
	) : socket_(io_service, udp::endpoint(udp::v4(), sport))
	{
		spdlog::set_pattern("[%t] %^[%l]%$ %v");

		spdlog::critical("Created UDPService");
		spdlog::error("Created UDPService");
		spdlog::info("Created UDPService");
		spdlog::debug("Created UDPService");
		spdlog::trace("Created UDPService");

		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), host, cport);
		xplane_endpoint_ = *resolver.resolve(query);
		start_receive();
	}

	void send(unsigned char* msg, int size)
	{
		socket_.send_to(boost::asio::buffer(msg, size), xplane_endpoint_);
	}

	void async_send(unsigned char* msg, int size)
	{
		socket_.async_send_to(boost::asio::buffer(msg, size), xplane_endpoint_,
			boost::bind(&UDPService::handle_send, this,
				msg,
				size,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}
	
	void init_service() {

		LOG_DEBUG("init_service(): ")

		boost::thread th(&UDPService::run_service, this);
		th.detach();
		Sleep(100);
	}

private:
	void run_service()
	{
		LOG_DEBUG("run_service(): ")

		try {
			this->io_service.run();
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
	void start_receive()
	{		
		socket_.async_receive_from(
			boost::asio::buffer(recv_buffer_),
			remote_endpoint_,
			boost::bind(&UDPService::handle_receive, this,
				asio::placeholders::error,
				asio::placeholders::bytes_transferred));
	}
	void handle_receive(const boost::system::error_code& error, std::size_t size/*bytes_transferred*/)
	{
		LOG_DEBUG("run_service(): ")

		if (!error || error == boost::asio::error::message_size)
		{
			char header[6] = "";
			for (size_t i = 0; i < 5; i++)
				header[i] = recv_buffer_[i];
			
			//TODO: Hace esto mas de manera que acepte otro tipo de headers.
			const char a[] = "RREF,";
			if (strcmp(a, header) == 0)
			{
				int header_size = 5;
				int dref_in_filled_bytes = 1;
				int cuadrar = 0;
				unsigned char* data = reinterpret_cast<unsigned char*>(&dref_in);

				for (size_t i = header_size; i < size; i++)
				{
					data[i-header_size-cuadrar] = recv_buffer_[i];

					if (dref_in_filled_bytes == 8) {
						try
						{
							(*DATAREFS_MAP[dref_in.sender_index]).value_ = dref_in.flt_value;
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
			start_receive();
		} else { LOG_ERROR("handle_receive(): {}", error.message()) }
	}

	void handle_send(unsigned char* message/*message*/, int size,
		const boost::system::error_code& /*error*/, std::size_t /*bytes_transferred*/)
	{

	}

	boost::asio::io_service io_service;
	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	udp::endpoint xplane_endpoint_;
	unsigned char recv_buffer_[1500 + NULL_TERMINATE_SAFELY];

} udp_service("192.168.8.103", "49000", 17);

#endif /* UDPSERVER_H */