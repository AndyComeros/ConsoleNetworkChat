#include "../net_connection.h"

net_connection::net_connection(asio::io_context& context, TSQue<net_message>& msg_queue) : m_asio_context(context),
m_socket(context),
m_msg_in(msg_queue)
{
}

net_connection::~net_connection()
{
	Disconnect();
}

void net_connection::SendMessage(net_message& message)
{
	// there is probably something wrong here...
	asio::post([&, message]() {

		// empty que means no messages are being written
		// if its not empty we can assume WriteHeader has already been called
		bool qEmpty = m_msg_out.Empty();
		m_msg_out.PushBack(message);
		
		if (qEmpty)
			StartWriteHeader();
	});
}

void net_connection::Connect(const std::string& ip, int port)
{
	asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string(ip), port);
	m_socket.async_connect(endpoint, [&](const asio::error_code& ec) {
		
		if (!ec)
		{
			std::cout << "connected!" << std::endl;
		}
		else {
			std::cerr << "[net_connection]: " << ec.message() << std::endl;
		}
	});
}

void net_connection::Disconnect()
{
	if (IsConnected())
		asio::post(m_asio_context, [this]() { m_socket.close(); });
}

void net_connection::Start()
{
	StartReadHeader();
}

bool net_connection::IsConnected()
{
	return m_socket.is_open();
}

TSQue<net_message>& net_connection::Recieved()
{
	return m_msg_in;
}

asio::ip::tcp::socket& net_connection::Socket()
{
	return m_socket;
}

void net_connection::StartWriteHeader()
{
	asio::async_write(m_socket, asio::buffer(&m_msg_out.Front().header, sizeof(net_message_header)),
		[&](const asio::error_code& ec, size_t bytes) {

			if (!ec) {
				StartWriteBody();
			}
			else {
				//m_msg_out.PopFront();
				std::cerr << "[Connection::StartWriteBody]: " << ec.message() << std::endl;
			}
		});
}

void net_connection::StartWriteBody()
{
	asio::async_write(m_socket, asio::buffer(m_msg_out.Front().contents.data(), m_msg_out.Front().contents.size()),
		[&](const asio::error_code& ec, size_t bytes) {

			if (!ec) {
				m_msg_out.PopFront();

				if (!m_msg_out.Empty()) {

					StartWriteHeader();
				}
			}
			else {
				std::cerr << "[Connection::StartWriteBody]: " << ec.message() << std::endl;
			}
		});
}

void net_connection::StartReadHeader()
{
	m_socket.async_read_some(asio::buffer(&m_current_msg_in.header, sizeof(net_message_header)), [&](const asio::error_code& ec, size_t bytes)
		{
			if (!ec) {
				// allocate memory in m_current_message to make sure it can store entire message
				m_current_msg_in.contents.resize(m_current_msg_in.header.data_size);

				// probably should check if message has a payload at all?
				StartReadMessage();

			}
			else {
				std::cout << ec.message();
				std::cout << std::endl;
			}
		});
}

void net_connection::StartReadMessage()
{
	m_socket.async_read_some(asio::buffer(m_current_msg_in.contents.data(), m_current_msg_in.header.data_size), [&](const asio::error_code& ec, size_t bytes)
		{
			if (!ec) {
				StartReadHeader();
			}
			else {
				std::cerr << "[Connection::StartReadMessage]: " << ec.message() << std::endl;
			}
		});
}


