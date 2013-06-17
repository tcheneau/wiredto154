#ifndef WIREDTO154_MODELS_PHY_H
#define WIREDTO154_MODELS_PHY_H

class Phy {
	public:
		static double compute_BER(double sinr);
		static double compute_PER(double sinr, int packetlen);
};
#endif /* WIREDTO154_MODELS_PHY_H */
