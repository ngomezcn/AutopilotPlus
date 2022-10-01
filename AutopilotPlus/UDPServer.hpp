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

class UDPServer
{
public:
	
	UDPServer(
		boost::asio::io_service& io_service,
		const int& port
	): socket_(io_service, udp::endpoint(udp::v4(), port))
	{
		start_receive();
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
			}

			std::cout << rpos.dat_lon << std::endl;
			std::cout << rpos.dat_lat << std::endl;
			std::cout << rpos.y_agl_mtr << std::endl;


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

	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	unsigned char recv_buffer_[1500 + NULL_TERMINATE_SAFELY];
};

#endif /* UDPSERVER_H */