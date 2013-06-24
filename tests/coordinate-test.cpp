#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
// #define BOOST_TEST_MODULE coordinate test

#include "coordinate.hpp"

BOOST_AUTO_TEST_CASE( constructors_test ) {
	Coordinate2D a;

	BOOST_CHECK_EQUAL(a.get_x(), 0.0);
	BOOST_CHECK_EQUAL(a.get_y(), 0.0);
}
