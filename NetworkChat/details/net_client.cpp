#include "../net_client.h"

net_client::net_client() : m_connection(m_asio_context, m_messages)
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

void net_client::Connect(const std::string& ip, int port)
{
	m_connection.Connect(ip,port);
}

void net_client::Disconnect()
{
}
