#ifndef WIREDTO154_MODELS_MODULATION_H
#define WIREDTO154_MODELS_MODULATION_H

#include <string>
#include <iostream>

class Modulation {
		Modulation() {};
	public:
		static const std::string modulation;
		static double compute_BER(double sinr);
		static double compute_PER(double sinr, int packetlen);
};

#endif /* WIREDTO154_MODELS_MODULATION_H */
