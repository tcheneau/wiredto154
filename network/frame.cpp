#include "frame.hpp"

#include <boost/bind.hpp>

#define MSB_UINT16(x) ((x>>8) & 0xff)
#define LSB_UINT16(x) (x & 0xff)

udp::endpoint Frame::endpoint;

Frame::frame Frame::build_outbound_frame(const Node<>::node_ptr sender,
										  const Node<>::node_list good_nodes,
										  const Node<>::node_list bad_nodes,
										  const Frame::frame & message) {
	Frame::frame outbound_frame;
	uint16_t good_n_size = good_nodes.size();
	uint16_t bad_n_size = bad_nodes.size();

	// encode the list of nodes that should receive the packet correctly
	outbound_frame.push_back(OUTBOUND_FRAME);
	outbound_frame.push_back(MSB_UINT16(sender->get_id()));
	outbound_frame.push_back(LSB_UINT16(sender->get_id()));
	outbound_frame.push_back(MSB_UINT16(good_n_size));
	outbound_frame.push_back(LSB_UINT16(good_n_size));
	for(Node<>::node_list::const_iterator i = good_nodes.begin(); i != good_nodes.end(); ++i) {
		uint16_t node_id = (* i)->get_id();
		outbound_frame.push_back(MSB_UINT16(node_id));
		outbound_frame.push_back(LSB_UINT16(node_id));
	}

	// encode the list of nodes that should receive a damaged packet
	outbound_frame.push_back(MSB_UINT16(bad_n_size));
	outbound_frame.push_back(LSB_UINT16(bad_n_size));
	for(Node<>::node_list::const_iterator i = bad_nodes.begin(); i != bad_nodes.end(); ++i) {
		uint16_t node_id = (* i)->get_id();
		outbound_frame.push_back(MSB_UINT16(node_id));
		outbound_frame.push_back(LSB_UINT16(node_id));
	}

	// add the initial frame
	for(Frame::frame::const_iterator i = message.begin(); i != message.end(); ++i) {
		outbound_frame.push_back(*i);
	}

	return outbound_frame;
}

Frame::frame Frame::build_sim_end_frame()
{
	Frame::frame end_frame;

	end_frame.push_back(SIM_END);
	return end_frame;
}

void Frame::set_multicast_parameter(std::string &mcast_addr, int mcast_port) {
	endpoint = udp::endpoint(boost::asio::ip::address::from_string(mcast_addr),
							 mcast_port);
}

/* this send a message to a multicast destination */
void Frame::send_broadcast_async(const Frame::frame & message, udp::socket &socket) {
	socket.async_send_to(
				boost::asio::buffer(message, message.size()), endpoint,
		boost::bind(&Frame::handle_send_to,
					boost::asio::placeholders::error));
}

void Frame::send_broadcast_sync(const Frame::frame &message) {
	boost::asio::io_service io_service;
	udp::socket socket(io_service, udp::endpoint(endpoint.protocol(), 0));
	socket.send_to(boost::asio::buffer(message, message.size()), endpoint);
}
