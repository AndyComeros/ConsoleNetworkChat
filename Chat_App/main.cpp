#include "net.h"

int main() {

	net_client client;
	client.Connect("127.0.0.1", 1234);
	client.Start();

		
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


	return 0;
}