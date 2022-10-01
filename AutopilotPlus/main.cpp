#include <iostream>
#include "UDPService.h"
#include "DREF_IN.h"

using namespace boost;

int main()
{
	try
	{
		boost::asio::io_service io_service;
		UDPService udp_service(io_service, 
            "192.168.8.103", "49000",
             7777);
		std::cout << "Starting:" << std::endl;

	    char a[400] = "sim/flightmodel/position/latitude";
	    DREF_IN latitude(1, 5, a);
	    unsigned char* data = reinterpret_cast<unsigned char*>(&latitude);
	    unsigned char header[] = "RREF";
	    unsigned char msg[sizeof(header) + sizeof(DREF_IN)];

	    memcpy(msg, header, sizeof(header));
	    memcpy(msg + sizeof(header), data, sizeof(DREF_IN));

	    for (size_t i = 0; i < sizeof(msg); i++)
	    {
	       // std::cout << std::hex << (int)msg[i] << " ";
	    }
        udp_service.send(msg, sizeof(msg));

        io_service.run();


		//th.join();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	//boost::asio::io_service io_service;
    //UDPClient client(std::move(socket), "192.168.8.103", "55272");


    



     //client.send(a);


     //unsigned char a[] = { 205, 133, 91, 174, 110, 138, 0, 64 };

     ////boost::asio::io_service io_service;

     //boost::asio::io_service io_service;
     //try
     //{
     //	UDPServer server(io_service, 13);
     //	io_service.run();


   


     //}
     //catch (std::exception& e)
     //{
     //	std::cerr << e.what() << std::endl;
     //}

     //try
     //{
         /*boost::asio::io_service io_service;
         udp::resolver resolver(io_service);
         udp::resolver::query query(udp::v4(), "192.168.8.103", "59679");
         udp::endpoint receiver_endpoint = *resolver.resolve(query);

         udp::socket socket(io_service);
         socket.open(udp::v4());

         //boost::array<char, 4> send_buf = [];

         unsigned char recv_buffer_[] = { 205, 133, 91, 174, 110, 138, 0, 64 };
         socket.send_to(boost::asio::buffer(recv_buffer_), receiver_endpoint);
         socket.async_send_to(boost::asio::buffer(recv_buffer_), receiver_endpoint)*/

         /*boost::array<char, 128> recv_buf;
         udp::endpoint sender_endpoint;
         size_t len = socket.receive_from(
             boost::asio::buffer(recv_buf), sender_endpoint);

         std::cout.write(recv_buf.data(), len);
     }
     catch (std::exception& e)
     {
         std::cerr << e.what() << std::endl;
     }*/

    return 0;
}