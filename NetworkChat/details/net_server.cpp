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

	std::string strbuff;
	for (;;) {
		
		std::cin >> strbuff;

		for (auto& it : m_connections)
		{
			std::cout << "\n[Sending Data]: " << strbuff << std::endl;
			//it->Socket().write_some(asio::buffer(strbuff));

			net_message message;
			message.AddData(strbuff.c_str(), strbuff.size());

			it->SendMessage(message);
		}
	}
}

void net_server::Stop()
{
}

void net_server::StartAcceptConnection()
{
	m_newConnection = std::make_shared<net_connection>(m_asio_context, m_messages);
	m_acceptor.async_accept(m_newConnection->Socket(), [&](const asio::error_code& ec) {
		if (!ec)
		{
			m_connections.push_back(m_newConnection);
			m_newConnection->Start();
			std::cout << "Client connected!" << std::endl;
		}
		else {
			std::cout << "[StartAcceptConnection]: " << ec.message() << std::endl;
		}
		
		StartAcceptConnection();
	});
}
