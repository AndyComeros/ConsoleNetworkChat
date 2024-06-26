#include "net.h"

int main() {

	std::cout << "username: ";
	std::string username = "no_name";
	std::getline(std::cin, username);
	std::cout << "set username to: " << username << std::endl;

	std::cout << "host ip: ";
	std::string serverIP = "";
	std::getline(std::cin, serverIP);
	std::cout << "Attempting to connect to: " << serverIP << std::endl;


	net_client client;
	client.Connect(serverIP, 25567);
	client.Start();
	
	std::thread inputThread([&]() {

		std::string strbuff;

		for (;;) {
			std::getline(std::cin, strbuff);

			net_message message;
			message.header.type = 1;
			message.AddData(username.c_str(), username.length() + 1);
			message.AddData(strbuff.c_str(), strbuff.size());
			client.Connection().SendMessage(message);
		}
	});
	
	std::thread outputThread([&]() {
		TSQue<net_message>& messages = client.Messages();
		for (;;) {
			if (!messages.Empty()) {

				const net_message& msg = messages.Front();
				if (messages.Front().header.type == 1) {

					std::string senderName(msg.payload.data());
					std::cout << "[" << senderName << "]: " <<
						std::string(msg.payload.begin() + senderName.length(), msg.payload.end()) << std::endl;
				}
				messages.PopFront();
			}
		}
	});

	if (inputThread.joinable())
		inputThread.join();

	if (outputThread.joinable())
		outputThread.join();

	return 0;
}