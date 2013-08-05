#ifndef WIREDTO154_NETWORK_DISPATCHER_HPP
#define WIREDTO154_NETWORK_DISPATCHER_HPP

#include "node.hpp"
#include "simulation.hpp"

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

using boost::asio::ip::udp;

class Dispatcher {
private:
	static void parse_inbound_frame(const Node<>::node_ptr sender,
									const Frame::frame &message,
									Simulation &sim,
									udp::socket & socket);
public:
	static void dispatch(int port, const Frame::frame & message, udp::socket & socket);
};

#endif // WIREDTO154_NETWORK_DISPATCHER_HPP
