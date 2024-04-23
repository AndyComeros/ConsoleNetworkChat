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
	m_socket.async_read_some(asio::buffer(&m_current_message.header,sizeof(net_message_header)), [&](const asio::error_code& ec, size_t bytes) 
	{
		if (!ec) {
			std::cout << "recieved header: [" << bytes << "] bytes, expected: " << sizeof(net_message_header) << "\n";
			std::cout << "header info [Type]: " << m_current_message.header.type << " | [size: ]" << m_current_message.header.data_size << std::endl;
			// allocate memory in m_current_message to make sure it can store entire message
			m_current_message.contents.resize(m_current_message.header.data_size);

			// probably should check if message has a payload at all?
			StartReadMessage();
			
		}else{
			std::cout << ec.message();
			std::cout << std::endl;
			///std::cerr << "[Error code start_read_header]: " << ec.message() << std::endl;
		}
	});
}

void net_client::StartReadMessage()
{
	m_socket.async_read_some(asio::buffer(m_current_message.contents.data(), m_current_message.header.data_size), [&](const asio::error_code& ec, size_t bytes)
		{
			if (!ec) {

				std::cout.write(m_current_message.contents.data(), bytes);
				std::cout << std::endl;
				StartReadHeader();
			}
			else {
				std::cout << ec.message();
				std::cout << std::endl;
			}
	});

}
