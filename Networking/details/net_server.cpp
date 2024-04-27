#include "../net_server.h"

net_server::net_server(int port) : m_acceptor(m_asio_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(),port))
{
}

net_server::~net_server()
{
	if (m_asio_thread)
		if (m_asio_thread->joinable()) m_asio_thread->join();
}

void net_server::Start()
{
	std::cout << "started server..." << std::endl;
	StartAcceptConnection();
	m_asio_thread = std::make_unique<std::thread>([&]() { m_asio_context.run(); });
}

void net_server::Stop()
{
}

void net_server::StartAcceptConnection()
{
	m_newConnection = std::make_shared<net_connection>(m_asio_context, m_messages, m_nextID);
	m_acceptor.async_accept(m_newConnection->Socket(), [&](const asio::error_code& ec) {
		if (!ec)
		{
			m_connections.emplace(m_nextID,m_newConnection);
			++ m_nextID;

			m_newConnection->Start();
			std::cout << "Client connected!" << std::endl;
		}
		else {
			std::cout << "[StartAcceptConnection]: " << ec.message() << std::endl;
		}
		
		StartAcceptConnection();
	});
}

void net_server::SendMessage(const net_message& msg, connectionID id)
{
	if (m_connections.find(id) != m_connections.end())
		m_connections[id]->SendMessage(msg);
}

void net_server::BroadcastMessage(const net_message& msg)
{
	for (auto it : m_connections) {
		it.second->SendMessage(msg);
	}
}

TSQue<net_message>& net_server::Messages()
{
	return m_messages;
}

connection_map& net_server::Connections() {
	return m_connections;
}
