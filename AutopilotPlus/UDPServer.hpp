#ifndef UDPSERVER_H
#define UDPSERVER_H

#define NULL_TERMINATE_SAFELY 1

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <iomanip>
#include <sstream>
#include <boost/thread.hpp>

#include "RPOS.h"
#include "UDPServer.hpp"
#include "UDPClient.hpp"
#include "DREF_IN.h"
#include "sim.h"

using namespace boost;
using namespace boost::asio::ip;

class UDPServer
{
public:
	
	UDPServer(const int& port): socket_(io_service, udp::endpoint(udp::v4(), port))
	{
		boost::thread* th = new boost::thread(boost::bind(&UDPServer::run_service, this));


		char a[400] = "sim/flightmodel/position/latitude";
		DREF_IN latitude(1, 5, a);
		unsigned char* data = reinterpret_cast<unsigned char*>(&latitude);
		unsigned char header[] = "RREF";
		unsigned char msg[sizeof(header) + sizeof(DREF_IN)];

		memcpy(msg, header, sizeof(header));
		memcpy(msg + sizeof(header), data, sizeof(DREF_IN));

		boost::shared_ptr<std::string> message(new std::string("Just a response bro"));

		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), "192.168.8.103", "55272");
		//remote_endpoint_ = *resolver.resolve(query);

		socket_.async_send_to(boost::asio::buffer(msg, sizeof(msg)), *resolver.resolve(query),
			boost::bind(&UDPServer::handle_send, this, message,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));

		start_receive();
	}

	void run_service()
	{
		try {
				this->io_service.run();
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}

private:

	void start_receive()
	{
		 socket_.async_receive_from(
			boost::asio::buffer(recv_buffer_),
			remote_endpoint_,
			boost::bind(&UDPServer::handle_receive, this,
				asio::placeholders::error,
				asio::placeholders::bytes_transferred));
	}

	void handle_receive(const boost::system::error_code& error,
		std::size_t /*bytes_transferred*/)
	{
		if (!error || error == boost::asio::error::message_size)
		{
			unsigned char* data = reinterpret_cast<unsigned char*>(&rpos);
			for (size_t i = 0; i < sizeof(RPOS); i++)
			{
				data[i] = recv_buffer_[i];

				std::cout << std::hex << " ";
			}
			std::cout << std::endl;
			

			boost::shared_ptr<std::string> message(new std::string("Just a response bro"));

			socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
				boost::bind(&UDPServer::handle_send, this, message,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));

			start_receive();
		}
	}

	void handle_send(boost::shared_ptr<std::string> message/*message*/,
		const boost::system::error_code& /*error*/,
		std::size_t /*bytes_transferred*/)
	{
	}

	boost::asio::io_service io_service;
	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	unsigned char recv_buffer_[1500 + NULL_TERMINATE_SAFELY];
};

#endif /* UDPSERVER_H */