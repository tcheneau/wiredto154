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
	static void handle_send_to(const boost::system::error_code & error) { if (error) throw; }
	static udp::endpoint endpoint;
public:
	static void dispatch(int port, Frame::frame message, udp::socket & socket);
	static void send_broadcast(Frame::frame message, udp::socket & socket);

};

#endif // WIREDTO154_NETWORK_DISPATCHER_HPP
