#ifndef WIREDTO154_MODELS_IEEE802154_H
#define WIREDTO154_MODELS_IEEE802154_H

#include "modulation.hpp"

class Phy_IEEE802154_OQPSK: public Modulation {
	public:
		static const std::string modulation;
		static double compute_BER(double sinr);
		static double compute_PER(double sinr, int packetlen);
};

#endif
