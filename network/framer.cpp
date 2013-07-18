#include "framer.hpp"

#include "simulation.hpp"

#include <ostream>

void parse_inbound_frame(Server::frame & message);

void Framer::dispatch(int port, Server::frame message) {
	Simulation & sim = Simulation::get();
	assert(sim.nodes[port]);

	if (!message.size())
		return;

	switch(message[0]) {
	case Framer::INBOUND_FRAME:
		parse_inbound_frame(message);
		break;
	case Framer::SIM_STOP:
		sim.stop(sim.nodes[port]->get_id());
		break;
	default:
		std::cerr << "could not recognized frame format" << std::endl;
	}
}

void parse_inbound_frame(Server::frame & message) {
// TODO
}


