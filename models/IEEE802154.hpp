#ifndef WIREDTO154_MODELS_IEEE802154_H
#define WIREDTO154_MODELS_IEEE802154_H

#include "modulation.hpp"

class Phy_IEEE802154_OQPSK: public Modulation {
		static const std::string modulation ;
	public:
		static double compute_BER(double sinr);
		static double compute_PER(double sinr, int packetlen);
		static const std::string get_modulation(void) { return modulation; };
};

#endif
