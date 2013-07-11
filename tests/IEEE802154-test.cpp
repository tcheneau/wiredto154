#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "IEEE802154.hpp"

BOOST_AUTO_TEST_CASE( test ) {
	BOOST_CHECK_EQUAL(Phy_IEEE802154_OQPSK::compute_BER(0), 0.5);
	BOOST_CHECK_EQUAL(Phy_IEEE802154_OQPSK::name, "O-QPSK");

}
