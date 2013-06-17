#ifndef WIREDTO154_MODELS_IEEE802154_H
#define WIREDTO154_MODELS_IEEE802154_H

#include "phy.hpp"


class Phy_IEEE802154: public Phy {
	public:
		static double compute_BER(double sinr);
		static double compute_PER(double sinr, int packetlen);
};

#endif
