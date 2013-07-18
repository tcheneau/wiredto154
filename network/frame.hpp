#ifndef WIREDTO154_NETWORK_FRAME_HPP
#define WIREDTO154_NETWORK_FRAME_HPP

#include "node.hpp"

#include <stdint.h>
#include <vector>

class Frame {
public:
	typedef std::vector<uint8_t> frame;
	enum type { INBOUND_FRAME, SIM_STOP, /* from the clients */
			  OUTBOUND_FRAME, SIM_END /* from the server */ };
	static frame build_outbound_frame(const Node<>::node_list good,
									  const Node<>::node_list bad,
									  const frame & message);
};

#endif // WIREDTO154_NETWORK_FRAME_HPP
