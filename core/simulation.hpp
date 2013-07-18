#ifndef WIREDTO154_MODELS_SIMULATION_H
#define WIREDTO154_MODELS_SIMULATION_H

#include "coordinate.hpp"
#include "modulation.hpp"
#include "IEEE802154.hpp"
#include "node.hpp"
#include "pathlossmodel.hpp"
#include "server.hpp"

#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/shared_ptr.hpp>
#include <exception>
#include <string>
#include <utility>

class Simulation {
	public:
		typedef boost::mt19937 rng;
		typedef boost::uniform_real<double> uniform_dist;
		typedef boost::variate_generator<rng &, uniform_dist> vargen;

		/* first node list is the list of nodes that will receive the packet
		 * second node list is the list of nodes that will receive a damaged packet */
		typedef std::pair<Node<>::node_list, Node<>::node_list> reception_pair;
		static rng randomness;
		void load(const std::string & filename);
		static Simulation & get(void);
		void set_seed(int seed) { randomness.seed(seed); }
		void set_modulation(std::string & modname);
		void set_pathloss_model(std::string & model);
		Node<>::node_map nodes;
		std::string list_nodes(void);
		bool is_properly_configured(std::string & error_msg);
		enum reception_type { PACKET_OK, PACKET_CORRUPTED, PACKET_NOT_RECEIVED };
		reception_type receivePacket(Node<>::node_ptr sender,
                           Node<>::node_ptr receiver,
                           const std::string & msg);
		reception_pair whoReceivedPacket(Node<>::node_ptr sender,
										const std::string & msg);

		void stop(int node) { std::exit(EXIT_SUCCESS); } // TODO: send termination message to other nodes as well

		struct exception_on_simulation_loading: std::exception {
            char const * what() const throw() { return ""; }
		};
	private:
		bool initialized;
		void init(void);
		uniform_dist randvar;
		boost::shared_ptr<vargen> randgen;
		boost::asio::io_service io_service;
		boost::shared_ptr<PathLossModel> pathloss;
		Modulation::modulation_ptr modulation;
		Server::server_list servers;
		Simulation (): randvar(0.0, 1.0) {
			randgen = boost::shared_ptr<vargen>(new vargen(randomness, randvar));
			initialized = false; }
};


#endif /* WIREDTO154_MODELS_SIMULATION_H */
