#include "IEEE802154.hpp"
#include "coordinate.hpp"
#include "node.hpp"

#include <iostream>
#include <boost/lexical_cast.hpp>
#include <string>
#include <vector>

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
	cout << Phy_IEEE802154_OQPSK::modulation << endl;

	print_separator();
	cout << "Node tests " << endl;
	print_separator();
	cout << endl;

	string one("one");
	string two("two");
	Coordinate2D one_coord(0., 0.);
	Coordinate2D two_coord(0., 1.);
	Node<> a(one, one_coord, 4., -95.);
	Node<> b(two, two_coord, 4., -95.);

	cout << a << std::endl;
	cout << b << std::endl;

	cout << "distance between" << a.get_name()
		 << " and " << b.get_name()
		 << " is " << boost::lexical_cast<string>(a.distance(b))
		 << endl;


	print_separator();
	cout << "BER tests " << endl;
	print_separator();
	cout << endl;

	float ber_range [] = { -10, -5, 0, 5, 10, 15};

	vector<float> ber_vect(ber_range, ber_range + sizeof(ber_range) / sizeof(* ber_range));

	for (std::vector<float>::iterator it = ber_vect.begin(); it != ber_vect.end(); ++it)
		cout << "bit error rate at " << *it << "db: "
			 << boost::lexical_cast<string>(Phy_IEEE802154_OQPSK::compute_BER(*it))
			 << endl;

	print_separator();
	cout << "LogNormalShadowing tests " << endl;
	print_separator();
	cout << endl;

	return 0;
}
