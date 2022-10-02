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

#include "RPOS.h"
#include "xplane_types.h"
#include "DREF_IN.h"
#include "sim.h"

using namespace boost;
using namespace boost::asio::ip;

class UDPService
{
public:

	UDPService(
		const char* host,
		const char* cport,
		int sport
	) : socket_(io_service, udp::endpoint(udp::v4(), sport))
	{
		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), host, cport);
		xplane_endpoint_ = *resolver.resolve(query);
		
		boost::thread* th = new boost::thread(boost::bind(&UDPService::run_service, this));

		start_receive();
	}

	void run_service()
	{
		while (1) {
			try {
				this->io_service.run();
			}
			catch (const std::exception& e) {
				std::cerr << e.what() << std::endl;
			}
		}
	}

	void send(unsigned char* msg, int size)
	{
		std::cout << "Send " << size << " bytes" << std::endl;
		//for (size_t i = 0; i < size; i++)
		//	std::cout << std::hex << (int)msg[i] << " ";
		//std::cout << std::endl;
		socket_.async_send_to(boost::asio::buffer(msg, size), xplane_endpoint_,
			boost::bind(&UDPService::handle_send, this,
				msg,
				size,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}
private:

	void start_receive()
	{
		socket_.async_receive_from(
			boost::asio::buffer(recv_buffer_),
			remote_endpoint_,
			boost::bind(&UDPService::handle_receive, this,
				asio::placeholders::error,
				asio::placeholders::bytes_transferred));
	}
	void handle_receive(const boost::system::error_code& error,
		std::size_t /*bytes_transferred*/)
	{
		if (!error || error == boost::asio::error::message_size)
		{
			char header[6] = "";
			for (size_t i = 0; i < 5; i++)
				header[i] = recv_buffer_[i];
			
			const char a[] = "RREF,";

			if (strcmp(a, header) == 0)
			{
				unsigned char* dref_in_ = reinterpret_cast<unsigned char*>(&dref_in);

				int header_size = 5;
				for (size_t i = header_size; i < sizeof(DREF_IN)+header_size; i++)
				{
					dref_in_[i-header_size] = recv_buffer_[i];
				}
				std::cout << dref_in.sender_index << " - " << dref_in.flt_value << std::endl;

				(*DATAREFS_MAP[dref_in.sender_index]).value_ = dref_in.flt_value;
			}
			start_receive();
		}
	}


	void handle_send(unsigned char* message/*message*/, int size,
		const boost::system::error_code& /*error*/,
		std::size_t /*bytes_transferred*/)
	{
	}

	boost::asio::io_service io_service;
	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	udp::endpoint xplane_endpoint_;
	unsigned char recv_buffer_[1500 + NULL_TERMINATE_SAFELY];
};

#endif /* UDPSERVER_H */