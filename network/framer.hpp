#ifndef WIREDTO154_NETWORK_FRAMER_HPP
#define WIREDTO154_NETWORK_FRAMER_HPP

#include "server.hpp"

class Framer {
private:
	enum type { INBOUND_FRAME, SIM_STOP, /* from the clients */
			  OUTBOUND_FRAME, SIM_END /* from the server */ };

	type frame_type;


public:
	static void dispatch(int port, Server::frame message);
	static void send_broadcast(Server::frame message);
};

struct Header {
	uint8_t type;
	union {
		int test;
	};
};

#endif // FRAMER_HPP
