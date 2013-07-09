#ifndef WIREDTO154_MODELS_SIMULATION_H
#define WIREDTO154_MODELS_SIMULATION_H

#include "coordinate.hpp"
#include "modulation.hpp"
#include "node.hpp"
#include "pathlossmodel.hpp"
#include "server.hpp"

#include <boost/asio.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <map>
#include <string>

class Simulation {
	public:
		typedef boost::mt19937 rng;
		static rng randomness;
		void load(const std::string & filename);
		static Simulation & get(void);
		void set_seed(int seed) { randomness.seed(seed); }
		void set_modulation(Modulation & modulation) {}
		void set_pathloss_model(PathLossModel & pathloss) {}
		std::map<int, Node<>::node_ptr> nodes;
		bool is_properly_configured(std::string & error_msg);
		bool receivePacket(Node<> & sender,
						   Node<> & receiver,
						   const std::string & msg);
	private:
		bool initialized;
		void init(void);
		boost::asio::io_service io_service;
		boost::shared_ptr<PathLossModel> pathloss;
		boost::shared_ptr<Modulation> modulation;
		Server::server_list servers;
		Simulation () { randomness(); initialized = false; }
};

#endif /* WIREDTO154_MODELS_SIMULATION_H */
