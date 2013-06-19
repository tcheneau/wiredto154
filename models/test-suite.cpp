#include "IEEE802154.hpp"
#include "coordinate.hpp"
#include "node.hpp"

#include <iostream>
#include <boost/lexical_cast.hpp>
#include <string>

using namespace std;
using std::string;

static inline void print_separator(void) {
	cout << "------------------------------------------------------------"
		 << endl;
}

int main(int argc, char const* argv[])
{
	double result = Phy_IEEE802154_OQPSK::compute_BER(10);
	string str = boost::lexical_cast<string>(result);
	cout << str << endl;
	cout << Phy_IEEE802154_OQPSK::get_modulation() << endl;

	print_separator();
	cout << "Node tests " << endl;
	print_separator();
	cout << endl;

	Node<> a(string("one"), Coordinate2D(0., 0.), 4., -95.);
	Node<> b(string("two"), Coordinate2D(0., 1.), 4., -95.);

	cout << "distance between" << a.get_name()
		 << " and " << b.get_name()
		 << " is " << boost::lexical_cast<string>(a.distance(b));


	print_separator();
	cout << "BER tests " << endl;
	print_separator();
	cout << endl;

	print_separator();
	cout << "LogNormalShadowing tests " << endl;
	print_separator();
	cout << endl;

	return 0;
}
