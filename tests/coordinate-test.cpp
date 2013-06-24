#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
using boost::test_tools::output_test_stream;
#define BOOST_TEST_MODULE coordinate test

#include "coordinate.hpp"

#include <ostream>

BOOST_AUTO_TEST_CASE( constructors_test ) {
	Coordinate2D a;
	BOOST_CHECK_EQUAL(a.get_x(), 0.0);
	BOOST_CHECK_EQUAL(a.get_y(), 0.0);

	Coordinate2D b(3., 4.);
	BOOST_CHECK_EQUAL(b.get_x(), 3.0);
	BOOST_CHECK_EQUAL(b.get_y(), 4.0);
}

BOOST_AUTO_TEST_CASE( method_test ) {
	Coordinate2D a;
	Coordinate2D b(3., 4.);
	BOOST_CHECK_EQUAL(b.distance(&a), 5.0);
	BOOST_CHECK_EQUAL(a.distance(&a), 0.0);
}

BOOST_AUTO_TEST_CASE( ostream_test ) {
	output_test_stream output;
	Coordinate2D b(3., 4.);
	output << b;
	BOOST_CHECK( output.is_equal("x: 3\ny: 4"));
}

