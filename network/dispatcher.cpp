#include "dispatcher.hpp"

#include "simulation.hpp"

#include <boost/asio.hpp>
#include "boost/bind.hpp"

#ifdef DEBUG
#include <ostream>
#endif

static int broadcast_port = 10000;
udp::endpoint Dispatcher::endpoint(boost::asio::ip::address::from_string("224.1.1.1"),
								   broadcast_port);

void Dispatcher::dispatch(int port,
						  const Frame::frame & message,
						  udp::socket &socket) {
	Simulation & sim = Simulation::get();
	assert(sim.nodes[port]);

	if (!message.size()) {
#ifdef DEBUG
		std::cout << "frame does not contain any payload, dropping it"	 << std::endl;
#endif /* DEBUG */
		return;
}
	switch(message[0]) {
	case Frame::INBOUND_FRAME:
#ifdef DEBUG
		std::cout << "packet is an INBOUND frame" << std::endl;
#endif /* DEBUG */
		parse_inbound_frame(sim.nodes[port], message, sim, socket);
		break;
	case Frame::SIM_STOP:
#ifdef DEBUG
		std::cout << "packet is an SIMULATION STOP frame" << std::endl;
#endif /* DEBUG */
		send_broadcast(Frame::build_sim_end_frame(), socket);
		sim.stop(sim.nodes[port]->get_id());
		break;
	default:
#ifdef DEBUG
		std::cerr << "could not recognize frame format" << std::endl;
#endif /* DEBUG */
		break;
	}

}

/* this send a message to a multicast destination */
void Dispatcher::send_broadcast(const Frame::frame & message, udp::socket &socket) {
	socket.async_send_to(
				boost::asio::buffer(message, message.size()), endpoint,
		boost::bind(&Dispatcher::handle_send_to,
					boost::asio::placeholders::error));
}


void Dispatcher::parse_inbound_frame(const Node<>::node_ptr sender,
									 const Frame::frame & message,
									 Simulation & sim,
									 udp::socket & socket) {
	Frame::frame real_packet(message.begin() + 1, message.end());
	Simulation::reception_status good_bad_nodes;

	good_bad_nodes = sim.whoReceivedPacket(sender, real_packet);
#ifdef DEBUG
	std::cout << "list of nodes that will receive the frame correctly" << std::endl;
	for(Node<>::node_list::const_iterator i = good_bad_nodes.first.begin();
		i != good_bad_nodes.first.end();
		++i) {
		std::cout << (*i)->get_id() << std::endl;
	}
	std::cout << "list of nodes that will receive a damaged frame" << std::endl;
	for(Node<>::node_list::const_iterator i = good_bad_nodes.second.begin();
		i != good_bad_nodes.second.end();
		++i) {
		std::cout << (*i)->get_id() << std::endl;
	}
#endif
	// build a frame for sending it back to the multicast address
	Frame::frame outbound_frame = Frame::build_outbound_frame(good_bad_nodes.first,
															  good_bad_nodes.second,
															  real_packet);

	// send to multicast for real
	send_broadcast(outbound_frame, socket);
}

