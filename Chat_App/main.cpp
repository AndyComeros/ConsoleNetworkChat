#include "net.h"

int main() {

	bool is_server = false;

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