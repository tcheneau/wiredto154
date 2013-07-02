#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#define BOOST_TEST_MODULE simulation test

#include "simulation.hpp"

#include <iostream>

BOOST_AUTO_TEST_CASE( loading_test ) {
	Simulation & sim = Simulation::get();
	sim.load("simulation-test-topology-simple.xml");
	BOOST_CHECK_EQUAL(sim.nodes[10001]->get_id(), 1);
	std::cout << sim.nodes[10001]->get_type() << std::endl;
	BOOST_CHECK_EQUAL(sim.nodes[10001]->get_type(), "unknown");
}

