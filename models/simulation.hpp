#ifndef WIREDTO154_MODELS_SIMULATION_H

#include <boost/random/mersenne_twister.hpp>

class Simulation {
	public:
		Simulation & get(void);
		void init(void);
		typedef boost::mt19937 rng;
		static rng randomness;
	private:
		Simulation () { randomness(); }
};

#define WIREDTO154_MODELS_SIMULATION_H
#endif /* WIREDTO154_MODELS_SIMULATION_H */
