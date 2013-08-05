#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "models/IEEE802154.hpp"

BOOST_AUTO_TEST_CASE( test ) {
	Phy_IEEE802154_OQPSK modulation;
	BOOST_CHECK_EQUAL(modulation.compute_BER(0), 0.5);
	BOOST_CHECK_EQUAL(modulation.get_name(), "O-QPSK");

}
