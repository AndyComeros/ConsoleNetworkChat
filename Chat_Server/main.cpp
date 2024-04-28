#include "net.h"

int main() {

	net_server server(25567);
	server.Start();
	
	std::thread printThread([&]() {

		TSQue<net_message>& messages = server.Messages();
		for (;;) {

			if (!messages.Empty()) {

				const net_message& msg = messages.Front();

				if (msg.header.type == 1) {
					server.BroadcastMessage(msg);
				}
				messages.PopFront();
			}
		}
	});

	if (printThread.joinable())
		printThread.join();

	return 0;
}