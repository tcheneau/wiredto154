#include "IEEE802154.hpp"
#include <cmath>
#include <boost/math/special_functions/binomial.hpp>

double Phy_IEEE802154_OQPSK::compute_BER(double sinr) {
	double sum = 0;

	for(int k=2; k<=16; k++)
		sum += pow(-1, k) * boost::math::binomial_coefficient<double>(16, k) * exp(20 * sinr * (1./k - 1.));

	return 8./15 * 1./16 * sum;
}

/* packet len is in byte */
double Phy_IEEE802154_OQPSK::compute_PER(double sinr, int packetlen) {
	return 1. -  pow(1. - compute_BER(sinr), 8 * packetlen);
}

