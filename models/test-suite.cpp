#include <iostream>
#include <boost/lexical_cast.hpp>

#include "IEEE802154.hpp"

using namespace std;


int main(int argc, char const* argv[])
{
	double result = Phy_IEEE802154::compute_BER(10);
	string str = boost::lexical_cast<string>(result);
	cout << str << endl;
	return 0;
}
