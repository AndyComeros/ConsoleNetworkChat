
#include "net_server.h"
#include "net_client.h"

int main() {

	bool is_server = false;

	if (is_server) {
		net_server server(1234);
		server.start();
	}
	else {
		net_client client;
		client.connect("127.0.0.1", 1234);
		client.start();
	}



	return 0;
}