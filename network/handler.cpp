#include "handler.hpp"

#include "simulation.hpp"

#include <ostream>

static int broadcast_port = 10000;

void Dispatcher::dispatch(int port, Frame::frame message, udp::socket &socket) {
	Simulation & sim = Simulation::get();
	assert(sim.nodes[port]);

	if (!message.size())
		return;

	switch(message[0]) {
	case Frame::INBOUND_FRAME:
		parse_inbound_frame(sim.nodes[port], message, sim, socket);
		break;
	case Frame::SIM_STOP:
		sim.stop(sim.nodes[port]->get_id());
		break;
	default:
		std::cerr << "could not recognize frame format" << std::endl;
	}

}

void Dispatcher::send_broadcast(Frame::frame message, udp::socket &socket) {
	// TODO: send message to a multicast group on a specific port

}


void Dispatcher::parse_inbound_frame(const Node<>::node_ptr sender,
									 const Frame::frame & message,
									 Simulation & sim,
									 udp::socket & socket) {
	Frame::frame real_packet(message.begin() + 1, message.end());
	Simulation::reception_status good_bad_nodes;

	good_bad_nodes = sim.whoReceivedPacket(sender, real_packet);

	// build a frame for sending it back to the multicast address
	Frame::frame outbound_frame = Frame::build_outbound_frame(good_bad_nodes.first,
															  good_bad_nodes.second,
															  real_packet);

	// send to multicast for real
	send_broadcast(outbound_frame, socket);
}

