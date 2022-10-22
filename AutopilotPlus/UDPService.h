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

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

class UDPService 
{
public:
	
	UDPService(
		const char* xplane_host,
		const char* xplane_port,
		const char* local_port
	) : socket_(io_service, udp::endpoint(udp::v4(), atoi(local_port)))
	{
		LOG_INFO("Starting UDPService => System IP: {0}, X-Plane Port {1}, AutopilotPlus Port: {2}", xplane_host, xplane_port, local_port);

		udp::resolver resolver(io_service);
		const udp::resolver::query query(udp::v4(), xplane_host, xplane_port);
		xplane_endpoint_ = *resolver.resolve(query);
	}

	~UDPService() {
		LOG_DEBUG("UDPService: destructor");
	}

	void send(unsigned char* msg, int size)
	{
		/*	for (int i = 0; i < size; i++)
			{
				std::cout << (int)msg[i];
			}
			std::cout << std::endl;
			LOG_DEBUG("UDPServer: send(): msg (HEX): {0}, size: {1}", spdlog::to_hex(LOG_CHAR_ARRAY(msg, size)), size);*/

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
		start_async_receiver();

		boost::thread th(&UDPService::async_run_service, this);
		th.detach();
		Sleep(100);
	}


private:
	void async_run_service()
	{
		LOG_DEBUG("Starting async UDP receiver");
		try {
			this->io_service.run();
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			LOG_ERROR("Exception at async_run_service(): {0}", e.what());
		}
	}
public:
	void start_async_receiver()
	{
		socket_.async_receive_from(
			boost::asio::buffer(recv_buffer_),
			remote_endpoint_,
			boost::bind(&UDPService::handle_receive, this,
				asio::placeholders::error,
				asio::placeholders::bytes_transferred));
	}
	//void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred)
	//{
	//	start_async_receiver();
	//}
	
	virtual void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred)
	{
		//start_async_receiver();
	}

	//void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred)
	//{
	//	if (!error || error == boost::asio::error::message_size)
	//	{
	//		char header[6] = "";
	//		for (size_t i = 0; i < 5; i++)
	//			header[i] = recv_buffer_[i];

	//		//TODO: Hace esto mas de manera que acepte otro tipo de headers.
	//		const char a[] = "RREF,";
	//		if (strcmp(a, header) == 0)
	//		{
	//			constexpr int header_size = 5;
	//			int dref_in_filled_bytes = 1;
	//			int cuadrar = 0;
	//			const auto data = reinterpret_cast<unsigned char*>(&dref_in);

	//			for (size_t i = header_size; i < bytes_transferred; i++)
	//			{
	//				data[i - header_size - cuadrar] = recv_buffer_[i];

	//				if (dref_in_filled_bytes == 8) {
	//					try
	//					{
	//						(drefs_map[dref_in.sender_index])->value_ = dref_in.flt_value;
	//					}
	//					catch (const std::exception&)
	//					{
	//						std::cout << "No se ha podido actualizar la simvar" << std::endl;
	//						throw;
	//					}
	//					dref_in_filled_bytes = 0;
	//					cuadrar += 8;
	//				}
	//				dref_in_filled_bytes++;
	//			}
	//		}
	//		start_async_receiver();
	//	}
	//	else { LOG_ERROR("handle_receive(): {}", error.message()); }
	//}

	void handle_send(unsigned char* message/*message*/, int size,
		const boost::system::error_code& /*error*/, std::size_t /*bytes_transferred*/)
	{

	}

	boost::asio::io_service io_service;
	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	udp::endpoint xplane_endpoint_;
	unsigned char recv_buffer_[1500 + NULL_TERMINATE_SAFELY];

};

#endif /* UDP_SERVER_H */