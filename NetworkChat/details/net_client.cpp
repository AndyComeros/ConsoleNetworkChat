#include "../net_client.h"

net_client::net_client() : m_socket(m_asio_context)
{
}

net_client::~net_client()
{
}

void net_client::Start()
{
	readbuff = std::vector<char>(256);
	StartReadHeader();
	m_asio_context.run();
}

void net_client::Connect(const std::string& ip, int port)
{
	asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string(ip), port);
	m_socket.async_connect(endpoint, [&](const asio::error_code& ec) {

		if (!ec)
		{
			std::cout << "connected!" << std::endl;
		}
		else {
			std::cout << "[net_connection]: " << ec.message() << std::endl;
		}
		});
}

void net_client::Disconnect()
{
}

void net_client::StartReadHeader()
{
	m_socket.async_read_some(asio::buffer(readbuff,sizeof(readbuff)), [&](const asio::error_code& ec, size_t bytes) 
	{
		if (!ec) {
			std::cout.write(readbuff.data(), bytes);
			std::cout << std::endl;
			// << "[Recived]: " << bytes << " bytes:\n" << readbuff.data() << std::endl;
		}else{
			std::cout << ec.message();
			std::cout << std::endl;
			///std::cerr << "[Error code start_read_header]: " << ec.message() << std::endl;
		}
		//std::cout << "\n";
		StartReadHeader();
	});
}

void net_client::StartReadMessage()
{
}
