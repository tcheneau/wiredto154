#include "simulation.hpp"

#include <cstdlib>
#include <ctime>
#include <sstream>
#include <pugixml.hpp>

Simulation::rng Simulation::randomness(static_cast<unsigned int>(std::time(0)));

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

	std::string description_500kVSub_LOS =
		"500kV Substation (LOS), from the article 'Opportunities and Challenges of Wireless Sensor Networks in Smart Grid'";
	std::string description_500kVSub_NLOS =
		"500kV Substation (NLOS), from the article 'Opportunities and Challenges of Wireless Sensor Networks in Smart Grid'";
	std::string description_UTV_LOS =
		"Underground Transformer Vault (LOS), from the article 'Opportunities and Challenges of Wireless Sensor Networks in Smart Grid'";
	std::string description_UTV_NLOS =
		"Underground Transformer Vault (NLOS), from the article 'Opportunities and Challenges of Wireless Sensor Networks in Smart Grid'";
	std::string description_MPR_LOS =
		"Main Power Room (LOS), from the article 'Opportunities and Challenges of Wireless Sensor Networks in Smart Grid'";
	std::string description_MPR_NLOS =
		"Main Power Room (NLOS), from the article 'Opportunities and Challenges of Wireless Sensor Networks in Smart Grid'";



	/* data extracted from "Opportunities and Challenges of Wireless Sensor Networks in Smart Grid", Gungor et al. */
	PathLossModel::register_model(PathLossModel::pathloss_ptr(
									new LogNormalShadowing("500kVSub-LOS",
														   description_500kVSub_LOS,
														   1, /* d0 -- reference distance */
														   2.42, /* path loss parameter (eta) */
														   3.12, /* shadowing deviation (sigma) */
														   55, /* path loss at d0 */
														   -90, /* background noise */
														   randomness /* randomness source */)));
	PathLossModel::register_model(PathLossModel::pathloss_ptr(
									new LogNormalShadowing("500kVSub-NLOS",
														   description_500kVSub_NLOS,
														   1, /* d0 -- reference distance */
														   3.51, /* path loss parameter (eta) */
														   2.95, /* shadowing deviation (sigma) */
														   55, /* path loss at d0 */
														   -90, /* background noise */
														   randomness /* randomness source */)));
	PathLossModel::register_model(PathLossModel::pathloss_ptr(
									new LogNormalShadowing("UTV-LOS",
														   description_UTV_LOS,
														   1, /* d0 -- reference distance */
														   1.45, /* path loss parameter (eta) */
														   2.45, /* shadowing deviation (sigma) */
														   55, /* path loss at d0 */
														   -90, /* background noise */
														   randomness /* randomness source */)));
	PathLossModel::register_model(PathLossModel::pathloss_ptr(
									new LogNormalShadowing("UTV-NLOS",
														   description_UTV_NLOS,
														   1, /* d0 -- reference distance */
														   3.15, /* path loss parameter (eta) */
														   3.19, /* shadowing deviation (sigma) */
														   55, /* path loss at d0 */
														   -90, /* background noise */
														   randomness /* randomness source */)));
	PathLossModel::register_model(PathLossModel::pathloss_ptr(
									new LogNormalShadowing("MPR-LOS",
														   description_MPR_LOS,
														   1, /* d0 -- reference distance */
														   1.64, /* path loss parameter (eta) */
														   3.29, /* shadowing deviation (sigma) */
														   55, /* path loss at d0 */
														   -90, /* background noise */
														   randomness /* randomness source */)));
	PathLossModel::register_model(PathLossModel::pathloss_ptr(
									new LogNormalShadowing("MPR-NLOS",
														   description_MPR_NLOS,
														   1, /* d0 -- reference distance */
														   2.38, /* path loss parameter (eta) */
														   2.25, /* shadowing deviation (sigma) */
														   55, /* path loss at d0 */
														   -90, /* background noise */
														   randomness /* randomness source */)));

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

Simulation::reception_type Simulation::receivePacket(Node<>::node_ptr sender,
                               Node<>::node_ptr receiver,
							   const std::string & msg) {
	// compute the SNR at the receiver
	// apply the BER model to determine if the packets can be received
    // TODO: add the PHY preamble, or some other PHY header to the BERcomputation

	double sinr = pathloss->compute_SINR(sender, receiver);

	if (sender->get_txPower() - sinr < receiver->get_rxSensitivity())
		return PACKET_NOT_RECEIVED; // the receiver does not even pick up the signal

	double per = modulation->compute_PER(sinr, msg.size());

	if (per > (*randgen)())
		return PACKET_CORRUPTED;
	else
		return PACKET_OK;
}

Simulation::reception_pair Simulation::whoReceivedPacket(Node<>::node_ptr sender, const std::string &msg) {
	Node<>::node_list received_OK, received_garbage;

	// go through the list of all neighbors
	for (Node<>::node_map::const_iterator i = nodes.begin(); i != nodes.end(); ++i) {
		if (i->second == sender) // skip the sending node
			continue;
		switch (receivePacket(sender, i->second, msg)) {
			case PACKET_OK:
				received_OK.push_back(i->second);
				break;
			case PACKET_CORRUPTED:
				received_garbage.push_back(i->second);
		}
	}

	return reception_pair(received_OK, received_garbage);
}

std::string Simulation::list_nodes()
{
	std::ostringstream nodes_list;
	for(std::map<int, Node<>::node_ptr>::const_iterator i = nodes.begin();  i != nodes.end(); ++i) {
		nodes_list << std::string("on port ") << i->first << std::endl;
		nodes_list << * i->second << std::endl;

	}
	return nodes_list.str();
}

void Simulation::set_modulation(std::string & modname) {
	try{
		this->modulation = Modulation::lookup_modulation(modname);
	} catch (Modulation::ModulationNotFoundException & e) {
		std::cerr << "unable to find modulation scheme " << modname << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void Simulation::set_pathloss_model(std::string & model) {
	try{
		 this->pathloss = PathLossModel::lookup_pathloss(model);
	} catch (PathLossModel::PathLossNotFoundException & e) {
		std::cerr << "unable to find path loss model " << model << std::endl;
		std::exit(EXIT_FAILURE);
	}
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
