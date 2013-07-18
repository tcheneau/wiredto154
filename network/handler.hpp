#ifndef WIREDTO154_NETWORK_HANDLER_HPP
#define WIREDTO154_NETWORK_HANDLER_HPP

#include "node.hpp"
#include "simulation.hpp"

#include <boost/asio.hpp>

using boost::asio::ip::udp;

class Dispatcher {
private:
	static void parse_inbound_frame(const Node<>::node_ptr sender,
									const Frame::frame &message,
									Simulation &sim,
									udp::socket & socket);
public:
	static void dispatch(int port, Frame::frame message, udp::socket & socket);
	static void send_broadcast(Frame::frame message, udp::socket & socket);
};

#endif // WIREDTO154_NETWORK_HANDLER_HPP
