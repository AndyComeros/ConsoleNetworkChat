#include "net.h"

int main() {

	net_server server(1234);
	server.Start();
	std::string strbuff;
	for (;;) {

		std::cin >> strbuff;

		for (auto& it : server.Connections())
		{
			std::cout << "\n[Sending Data]: " << strbuff << std::endl;

			net_message message;
			message.header.type = 1;
			message.AddData("poggers",sizeof("poggers"));
			message.AddData(strbuff.c_str(), strbuff.size());

			it.second->SendMessage(message);
		}
	}
	return 0;
}