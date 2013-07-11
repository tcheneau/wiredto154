#ifndef WIREDTO154_MODELS_IEEE802154_H
#define WIREDTO154_MODELS_IEEE802154_H

/* this file defines the O-QPSK modulation scheme,
 * as defined in the IEEE 802.15.4-2006 standard */

#include "modulation.hpp"

class Phy_IEEE802154_OQPSK: public Modulation {
	protected:
		Phy_IEEE802154_OQPSK (void) {}
	public:
		static const std::string name;
		static const std::string description;
		static double compute_BER(double sinr);
		static double compute_PER(double sinr, int packetlen);
		static void init(void) { available_modulations[name] = Modulation::modulation_ptr(new Phy_IEEE802154_OQPSK()); }
};

#endif
