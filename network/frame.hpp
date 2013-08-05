#ifndef WIREDTO154_NETWORK_FRAME_HPP
#define WIREDTO154_NETWORK_FRAME_HPP

#include "models/node.hpp"

#include <boost/asio.hpp>
#include <string>
#include <stdint.h>
#include <vector>

using boost::asio::ip::udp;

class Frame {
	static udp::endpoint endpoint;
	static void handle_send_to(const boost::system::error_code & error) { if (error) throw; }
public:
	typedef std::vector<uint8_t> frame;
	enum type { INBOUND_FRAME, SIM_STOP, /* from the clients */
			  OUTBOUND_FRAME, SIM_END, /* from the server */
			  MISC_DATA = 128 /* type for events registered by the simulation logger */ };
	static frame build_outbound_frame(const Node<>::node_ptr sender,
									  const Node<>::node_list good,
									  const Node<>::node_list bad,
									  const frame & message);
	static frame build_sim_end_frame(void);
	static void set_multicast_parameter(std::string & mcast_addr, int mcast_port);
	static void send_broadcast_async(const Frame::frame & message, udp::socket & socket);
	static void send_broadcast_sync(const Frame::frame & message);
};

#endif // WIREDTO154_NETWORK_FRAME_HPP
