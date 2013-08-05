#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/random/mersenne_twister.hpp>

#include "models/coordinate.hpp"
#include "models/node.hpp"
#include "models/pathlossmodel.hpp"

#include <string>

BOOST_AUTO_TEST_CASE( test_lognormalshadowing ) {
	boost::mt19937 rng;
	LogNormalShadowing model(std::string("test"), std::string("test"), 1., 1., 2., 10., -94., rng);
	Coordinate2D c1(0, 10);
	Coordinate2D c2;
	Node<>::node_ptr n1(new Node<> (1, c1, 4, -90, std::string("test"), std::string("test-node")));
	Node<>::node_ptr n2(new Node<> (2, c2, 4, -90, std::string("test"), std::string("test-node")));


	double sinr1 = model.compute_SINR(n1, n2);
	double sinr2 = model.compute_SINR(n1, n2);

	BOOST_CHECK(sinr1 != sinr2);
}
