#ifndef WIREDTO154_MODELS_MODULATION_H
#define WIREDTO154_MODELS_MODULATION_H

#include <boost/shared_ptr.hpp>
#include <exception>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Modulation {
	public:
		typedef boost::shared_ptr<Modulation> modulation_ptr;
		struct ModulationNotFoundException: public std::exception {};
	protected:
		Modulation() {};
		static std::map<std::string, modulation_ptr> available_modulations;
	public:
		static const std::string name;
		static const std::string description;
		static double compute_BER(double sinr);
		static double compute_PER(double sinr, int packetlen);
		static const std::string & get_description(void) { return description; }
		static modulation_ptr lookup_modulation(std::string & modname);
		static std::vector<std::string> list_available_modulation(void);
		static void init(void) {}
};

#endif /* WIREDTO154_MODELS_MODULATION_H */
