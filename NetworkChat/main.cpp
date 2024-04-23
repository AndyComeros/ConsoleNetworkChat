
#include "net_server.h"
#include "net_client.h"
#include "ts_queue.h"

int main() {


	//net_message message;
	//
	//char words[] = { "hello" };
	//
	//message.AddData(words, sizeof(words));
	//
	//std::cout << message.header.type << std::endl;
	//std::cout << message.header.data_size << std::endl;
	//std::cout << message.TotalSize() << std::endl;
	//std::cout << std::string(message.contents.begin(), message.contents.end()) << std::endl;

	bool is_server = true;
	
	if (is_server) {
		net_server server(1234);
		server.Start();
	}
	else {
		net_client client;
		client.Connect("127.0.0.1", 1234);
		client.Start();
	}

	return 0;
}