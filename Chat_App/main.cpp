#include "net.h"

int main() {

	net_client client;

	client.Connect("127.0.0.1", 62225);
	client.Start();

	std::cout << "username: ";
	std::string username = "no_name";
	std::getline(std::cin, username);
	std::cout << "set username to: " << username << std::endl;

	std::thread inputThread([&]() {

		std::string strbuff;

		for (;;) {

			//std::cin >> strbuff;
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

					std::string senderName(msg.contents.data());
					std::cout << "[" << senderName << "]" <<
						std::string(msg.contents.begin() + senderName.length(), msg.contents.end()) << std::endl;
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