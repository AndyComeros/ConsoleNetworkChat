#include "../net_server.h"

net_server::net_server(int port) : m_acceptor(m_asio_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(),port))
{
}

net_server::~net_server()
{
	if (m_asio_thead)
		if (m_asio_thead->joinable()) m_asio_thead->join();
}

void net_server::Start()
{
	std::cout << "started server..." << std::endl;
	StartAcceptConnection();
	m_asio_thead = std::make_unique<std::thread>([&]() { m_asio_context.run(); });

	std::string strbuff;
	for (;;) {
		
		std::cin >> strbuff;

		for (auto& it : m_connections)
		{
			std::cout << "\n[Sending Data]: " << strbuff << std::endl;
			it->Socket().write_some(asio::buffer(strbuff));
		}
	}
}

void net_server::Stop()
{
}

void net_server::StartAcceptConnection()
{
	m_newConnection = std::make_shared<net_connection>(m_asio_context, m_message_in);
	m_acceptor.async_accept(m_newConnection->Socket(), [&](const asio::error_code& ec) {
		if (!ec)
		{
			m_connections.push_back(m_newConnection);
			std::cout << "Client connected!" << std::endl;
		}
		else {
			std::cout << "[start_accept_connection]: " << ec.message() << std::endl;
		}
		
		StartAcceptConnection();
	});
}
