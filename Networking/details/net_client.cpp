#include "../net_client.h"

net_client::net_client() : m_connection(m_asio_context, m_messages, 0)
{
}

net_client::~net_client()
{
	if (m_asio_thread)
		if (m_asio_thread->joinable()) m_asio_thread->join();
}

void net_client::Start()
{
	m_connection.Start();
	m_asio_thread = std::make_unique<std::thread>([&]() { m_asio_context.run(); });
}

void net_client::Connect(const std::string& ip, uint16_t port)
{
	m_connection.Connect(ip,port);
}

void net_client::Disconnect()
{
}

TSQue<net_message>& net_client::Messages()
{
	return m_messages;
}

net_connection& net_client::Connection()
{
	return m_connection;
}

void net_client::OnConnect()
{
}
