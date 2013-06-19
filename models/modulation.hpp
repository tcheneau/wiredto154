#ifndef WIREDTO154_MODELS_MODULATION_H
#define WIREDTO154_MODELS_MODULATION_H

#include <string>

class Modulation {
		static const std::string modulation;
	public:
		static double compute_BER(double sinr);
		static double compute_PER(double sinr, int packetlen);
		static const std::string get_modulation(void);
};

#endif /* WIREDTO154_MODELS_MODULATION_H */
