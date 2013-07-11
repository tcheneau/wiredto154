#include "simulation.hpp"

#include <pugixml.hpp>


Simulation::rng Simulation::randomness;

Simulation & Simulation::get(void) {
	static Simulation sim;
	if (!sim.initialized)
		sim.init();
	return sim;

}

void Simulation::init(void) {
	this->initialized = true;

	/* all models need to be initialized here */
	Modulation::register_modulation(Modulation::modulation_ptr(new Phy_IEEE802154_OQPSK()));
}

void Simulation::load(const std::string & filename) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename.c_str());
	pugi::xml_node xml_nodes;

	if (!result) {
		std::cerr << "Error description: " << result.description() << "\n";
		throw exception_on_simulation_loading();
	}

	nodes.clear();
	servers.clear();

	xml_nodes = doc.child("simulation").child("nodes");
	if (!xml_nodes)
		throw "XML file contains no node description";

	for (pugi::xml_node node = xml_nodes.child("node"); node; node = node.next_sibling("node"))
	{
		pugi::xml_attribute node_id = node.attribute("id");
		pugi::xml_attribute node_port = node.attribute("port");
		pugi::xml_attribute node_x = node.attribute("x");
		pugi::xml_attribute node_y = node.attribute("y");
		pugi::xml_attribute node_txPower = node.attribute("txPower");
		pugi::xml_attribute node_rxSensitivity = node.attribute("rxSensitivity");
		std::string node_description(node.attribute("description").value());
		std::string node_type(node.attribute("type").value());

		if (!node_id)
			throw "unable to obtain ID attribute";
		if (!node_port)
			throw "unable to obtain port attribute";
		if (!node_x)
			throw "unable to obtain x attribute";
		if (!node_y)
			throw "unable to obtain y attribute";
		if (!node_txPower)
			throw "unable to obtain txPower attribute";
		if (!node_rxSensitivity)
			throw "unable to obtain rxSensitivity attribute";
		if (node_description.empty())
			node_description.assign("no description available");
		if (node_type.empty())
			node_type.assign("unknown");


		nodes[node_port.as_int()] = Node<>::node_ptr(new Node<>(node_id.as_int(),
					Coordinate2D(node_x.as_float(), node_y.as_float()),
					node_txPower.as_float(),
					node_rxSensitivity.as_float(),
					node_description,
					node_type)

				);

		servers.push_back(Server::server_ptr(new Server(io_service, node_port.as_int())));
	}

	// start a server on each ports
}

bool Simulation::receivePacket(Node <> & sender,
							   Node <> & receiver,
							   const std::string & msg) {
	// compute the SNR at the receiver
	// apply the BER model to determine if the packets can be received
	return true;
}

void Simulation::set_modulation(std::string & modname) {
	modulation = Modulation::lookup_modulation(modname);
}

bool Simulation::is_properly_configured(std::string & error_msg) {
	if (!initialized) {
		error_msg = "Simulation has not been initialized properly";
		return false;
	}
	if (!pathloss) {
		error_msg = "Propagation model is not set";
		return false;
	}

	if (!modulation) {
		error_msg = "Modulation scheme is not set";
		return false;
	}

	return true;

}
