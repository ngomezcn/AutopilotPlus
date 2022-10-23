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
	
	UDPService(
		const char* target_ip,
		const char* target_port,
		const char* local_port
	) : socket_(io_service, udp::endpoint(udp::v4(), atoi(local_port)))
	{
		Log::info("Starting UDPService => Target IP: {0}, Target Port {1}, Local Port: {2}", target_ip, target_port, local_port);

		udp::resolver resolver(io_service);
		const udp::resolver::query query(udp::v4(), target_ip, target_port);
		xplane_endpoint_ = *resolver.resolve(query);
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

	boost::thread th_async_run_service;
	std::atomic_bool terminate_async_receiver = false;

	void init_udp_service() {
		start_async_receiver();

		th_async_run_service = boost::thread(&UDPService::async_run_service, this);
		th_async_run_service.detach();
		Sleep(50);
	}

	~UDPService()
	{
		terminate_async_receiver = true;
		th_async_run_service.join();
		Log::debug("UDPService: destructor");
	}

private:
	void async_run_service()
	{
		if(!terminate_async_receiver)
		{
			Log::info("Starting async UDP receiver");
			try {
				this->io_service.run();
			}
			catch (const std::exception& e) {
				Log::error("Exception at async_run_service(): {0}", e.what());
				Log::info("Stopping async UDP receiver");  return;
			}
		} else
		{
			Log::info("Stopping async UDP receiver");  return;
		}
	}
public:
	void start_async_receiver()
	{
		socket_.async_receive_from(
			boost::asio::buffer(received_buffer),
			remote_endpoint_,
			boost::bind(&UDPService::handle_receive, this,
				asio::placeholders::error,
				asio::placeholders::bytes_transferred));
	}
	
	virtual void handle_receive(const boost::system::error_code& error, std::size_t size_in_bytes)
	{
		
	}

	void handle_send(unsigned char* message, int size, const boost::system::error_code& error, std::size_t size_in_bytes)
	{
		std::cout << message << error.to_string() << std::endl;
	}

	boost::asio::io_service io_service;
	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	udp::endpoint xplane_endpoint_;
	unsigned char received_buffer[1500 + NULL_TERMINATE_SAFELY];

};

#endif /* UDP_SERVER_H */