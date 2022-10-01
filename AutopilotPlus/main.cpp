#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <iomanip>
#include <sstream>
#include <sstream>
#include <string>

using namespace boost;
using namespace boost::asio::ip;

#include "UDPServer.hpp"
#include "UDPClient.hpp"
#include "DREF_IN.h"
#include "sim.h"



void convert_to_hex_string(std::ostringstream& op, const unsigned char* data, int size)
{
    // Format flags
    std::ostream::fmtflags old_flags = op.flags();

    // Fill characters
    char old_fill = op.fill();
    op << std::hex << std::setfill('\0');

    for (int i = 0; i < size; i++)
    {
        // Give space between two hex values
        if (i > 0)
            op << ' ';

        // force output to use hex version of ascii code
        op << std::setw(2) << static_cast<int>(data[i]);
    }

    op.flags(old_flags);
    op.fill(old_fill);
}


int main()
{
    boost::asio::io_service io_service;
    UDPClient client(io_service, "192.168.8.103", "59679");

    char a[400] = "sim/flightmodel/position/latitude";
    DREF_IN latitude(1, 5, a, sizeof(a));

    unsigned char* data = reinterpret_cast<unsigned char*>(&latitude);
	unsigned char header[] = "RREF";
	unsigned char msg[sizeof(header) + sizeof(DREF_IN)];

    memcpy(msg, header, sizeof(header));
	memcpy(msg + sizeof(header), data, sizeof(DREF_IN));

	std::cout << "DREF_IN: " << sizeof(DREF_IN) << std::endl;
	for (size_t i = 0; i < sizeof(msg); i++)
	{
		std::cout << std::hex << (int)msg[i] << " ";
	}

    mmain();

   /* std::ostringstream op;
    convert_to_hex_string(op, reinterpret_cast<const unsigned char*>(&latitude), sizeof(DREF_IN));
    std::string output = op.str();
    std::cout << "After conversion from struct to hex string:\n" << output << std::endl;*/


    //client.send(a);


    //unsigned char a[] = { 205, 133, 91, 174, 110, 138, 0, 64 };

	////boost::asio::io_service io_service;

	//boost::asio::io_service io_service;
	//try
	//{
	//	UDPServer server(io_service, 13);
	//	io_service.run();


	//	//UDPClient client(io_service, "localhost", "1337");

	//	//client.send("Hello, World!");


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