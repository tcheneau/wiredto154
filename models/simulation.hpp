#ifndef WIREDTO154_MODELS_SIMULATION_H

#include "coordinate.hpp"
#include "node.hpp"

#include <boost/random/mersenne_twister.hpp>
#include <map>

class Simulation {
	public:
		typedef boost::mt19937 rng;
		static rng randomness;
		std::map<int, Node<>::node_ptr> nodes;
		void load(const char * filename);
		static Simulation & get(void);
		void set_seed(int seed) { randomness.seed(seed); }
	private:
		bool initialized;
		void init(void);
		Simulation () { randomness(); initialized = false; }
};

#define WIREDTO154_MODELS_SIMULATION_H
#endif /* WIREDTO154_MODELS_SIMULATION_H */
