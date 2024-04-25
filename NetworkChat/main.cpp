
#include "net_server.h"
#include "net_client.h"
#include "ts_queue.h"

int main() {

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