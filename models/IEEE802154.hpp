#ifndef WIREDTO154_MODELS_IEEE802154_H
#define WIREDTO154_MODELS_IEEE802154_H

/* this file defines the O-QPSK modulation scheme,
 * as defined in the IEEE 802.15.4-2006 standard */

#include "modulation.hpp"

class Phy_IEEE802154_OQPSK: public Modulation {
	public:
		Phy_IEEE802154_OQPSK(void) {
			name = "O-QPSK";
			description = "defined in IEEE 802.15.4-2006, pages 267-269";
		}
		double compute_BER(double sinr);
		double compute_PER(double sinr, int packetlen);
};

#endif
