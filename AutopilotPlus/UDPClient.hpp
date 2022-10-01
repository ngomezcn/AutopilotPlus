#ifndef UDPCLIENT_H
#define UDPCLIENT_H

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

class UDPClient
{
public:

	UDPClient(
		boost::asio::io_service& io_service,
		const char* host,
		const char* port
	) : socket_(io_service, udp::endpoint(udp::v4(), 7777))
	{
		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), host, port);
		remote_endpoint_ = *resolver.resolve(query);
	}

	void send(unsigned char *msg, int size)
	{
		std::cout << "We send " << size << " bytes" << std::endl;
		boost::shared_ptr<std::string> message(new std::string("Just a response bro"));

		socket_.async_send_to(boost::asio::buffer(msg, size), remote_endpoint_,
			boost::bind(&UDPClient::handle_send, this, message,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}
private:
	void handle_send(boost::shared_ptr<std::string> message/*message*/,
		const boost::system::error_code& /*error*/,
		std::size_t /*bytes_transferred*/)
	{
	}

	udp::socket socket_;
	udp::endpoint remote_endpoint_;
};

#endif /* UDPCLIENT_H */