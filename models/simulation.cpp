#include "simulation.hpp"

Simulation & Simulation::get(void) {
	static Simulation sim;
	return sim;

}

void Simulation::init(void) {
	Simulation & sim = Simulation::get();
	// TBD: initialisation code
}

