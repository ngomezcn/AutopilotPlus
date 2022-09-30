
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

void convert_to_struct(std::istream& ip, unsigned char* data, int size)
{
	// Get the line we want to process
	std::string line;
	std::getline(ip, line);

	std::istringstream ip_convert(line);
	ip_convert >> std::hex;

	// Read in unsigned ints, as wrote out hex version
	// of ascii code
	unsigned int u = 0;
	int i = 0;

	while ((ip_convert >> u) && (i < size))
		if ((0x00 <= u) && (0xff >= u))
			data[i++] = static_cast<unsigned char>(u);
}

class udp_server
{
public:
	udp_server(udp::socket socket)
		: socket_(std::move(socket)) {

		start_receive();
	}

private:
	void start_receive()
	{
		socket_.async_receive_from(
			boost::asio::buffer(recv_buffer_),
			remote_endpoint_,
			boost::bind(&udp_server::handle_receive, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));

		unsigned char* data = reinterpret_cast<unsigned char*>(&rpos);

		for (size_t i = 0; i < sizeof(RPOS); i++)
		{
			data[i] = recv_buffer_[i];
		}

		std::cout << rpos.dat_lon << std::endl;
	}

	void handle_receive(const boost::system::error_code& error,
		std::size_t /*bytes_transferred*/)
	{
		if (!error || error == boost::asio::error::message_size)
		{
			boost::shared_ptr<std::string> message(new std::string("Just a response bro"));

			socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
				boost::bind(&udp_server::handle_send, this, message,
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
	//boost::array<unsigned char, 4> recv_buffer_;
	//char recv_buffer_[1500 + 1];
	unsigned char recv_buffer_[1500 + 1];
};

int main()
{
	try
	{
		boost::asio::io_service io_service;

		udp::socket socket(io_service, udp::endpoint(udp::v4(), 13));
		udp_server server(std::move(socket));
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}