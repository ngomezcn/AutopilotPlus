// ReSharper disable CppClangTidyModernizeAvoidBind
#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#define NULL_TERMINATE_SAFELY 1

#include <iostream>
#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include "logger.h"
#include "DREFs_definition.h"

using namespace boost;
using namespace boost::asio::ip;

class UDPService 
{
public:

	boost::thread th_async_run_service;
	std::atomic_bool terminate_async_receiver = false;

	boost::asio::io_service io_service;
	udp::socket socket_;
	unsigned char received_buffer[1500 + NULL_TERMINATE_SAFELY];

	udp::endpoint remote_endpoint_;
	udp::endpoint xplane_endpoint_;

	UDPService(const char* target_ip, const char* target_port, const char* local_port);
	~UDPService();

	void send(unsigned char* msg, int size);

	void async_send(unsigned char* msg, int size);

	void handle_send(unsigned char* message, int size, const boost::system::error_code& error, std::size_t size_in_bytes);

	void start_udp_receiver();

	void start_io_udp_service();

	void async_receive();

	virtual void handle_receive(const boost::system::error_code& error, std::size_t size_in_bytes);
};

inline UDPService::UDPService(const char* target_ip, const char* target_port, const char* local_port): socket_(io_service, udp::endpoint(udp::v4(), atoi(local_port)))
{
	Log::info("Starting UDPService => Target IP: {0}, Target Port {1}, Local Port: {2}", target_ip, target_port, local_port);

	udp::resolver resolver(io_service);
	const udp::resolver::query query(udp::v4(), target_ip, target_port);
	xplane_endpoint_ = *resolver.resolve(query);
}

inline void UDPService::send(unsigned char* msg, int size)
{
	socket_.send_to(boost::asio::buffer(msg, size), xplane_endpoint_);
}

inline void UDPService::async_send(unsigned char* msg, int size)
{
	socket_.async_send_to(boost::asio::buffer(msg, size), xplane_endpoint_,
	                      boost::bind(&UDPService::handle_send, this,
	                                  msg,
	                                  size,
	                                  boost::asio::placeholders::error,
	                                  boost::asio::placeholders::bytes_transferred));
}

inline void UDPService::start_udp_receiver()
{
	async_receive();

	th_async_run_service = boost::thread(&UDPService::start_io_udp_service, this);
	th_async_run_service.detach();
	Sleep(50);
}

inline UDPService::~UDPService()
{
	terminate_async_receiver = true;
	th_async_run_service.join();
	Log::debug("UDPService: destructor");
}

inline void UDPService::start_io_udp_service()
{
	if(!terminate_async_receiver)
	{
		Log::info("Starting async UDP receiver");
		try {
			this->io_service.run();
		}
		catch (const std::exception& e) {
			Log::error("Exception at start_io_udp_service(): {0}", e.what());
			Log::info("Stopping async UDP receiver");  return;
		}
	} else
	{
		Log::info("Stopping async UDP receiver");  return;
	}
}

inline void UDPService::async_receive()
{
	socket_.async_receive_from(
		boost::asio::buffer(received_buffer),
		remote_endpoint_,
		boost::bind(&UDPService::handle_receive, this,
		            asio::placeholders::error,
		            asio::placeholders::bytes_transferred));
}

inline void UDPService::handle_receive(const boost::system::error_code& error, std::size_t size_in_bytes)
{
		
}

inline void UDPService::handle_send(unsigned char* message, int size, const boost::system::error_code& error,
	std::size_t size_in_bytes)
{
	std::cout << message << error.to_string() << std::endl;
}

#endif /* UDP_SERVER_H */
