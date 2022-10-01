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

#include "RPOS.h"

using namespace boost;
using namespace boost::asio::ip;

class UDPService
{
public:

	UDPService(
		boost::asio::io_service& io_service,
		const char* host,
		const char* cport,
		int sport
	) : socket_(io_service, udp::endpoint(udp::v4(), sport))
	{

		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), host, cport);
		xplane_endpoint_ = *resolver.resolve(query);
		
		start_receive();
	}

	void send(unsigned char* msg, int size)
	{
		std::cout << "We send " << size << " bytes" << std::endl;

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
		std::cout << remote_endpoint_.port() << std::endl;
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
		std::cout << remote_endpoint_.port() << std::endl;

		if (!error || error == boost::asio::error::message_size)
		{
			unsigned char* data = reinterpret_cast<unsigned char*>(&recv_buffer_);

			for (size_t i = 0; i < 5; i++)
			{
				std::cout << recv_buffer_[i] << " ";
			}
			std::cout << std::endl;

			start_receive();
		}
	}


	void handle_send(unsigned char* message/*message*/, int size,
		const boost::system::error_code& /*error*/,
		std::size_t /*bytes_transferred*/)
	{
	}

	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	udp::endpoint xplane_endpoint_;
	unsigned char recv_buffer_[1500 + NULL_TERMINATE_SAFELY];
};

#endif /* UDPSERVER_H */