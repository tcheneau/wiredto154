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
		typedef std::map<std::string, modulation_ptr> modulation_map;
	private:
		Modulation(const Modulation &) {}
		void operator=(const Modulation &) {}

		static modulation_map available_modulations;
	protected:
		std::string name;
		std::string description;
		Modulation(): name("Unknown Modulation"), description("") {}
	public:
		struct ModulationAlreadyRegistered: public std::exception {};
		struct ModulationNotFoundException: public std::exception {};

		virtual const std::string& get_name(void) const { return name; }
		virtual const std::string& get_description(void) const { return description; }
		virtual double compute_BER(double sinr) = 0;
		virtual double compute_PER(double sinr, int packetlen) = 0;
		virtual const std::string & get_description(void) { return description; }
		static modulation_ptr lookup_modulation(std::string & modname);
		static void register_modulation(modulation_ptr);
		static std::vector<std::string> list_available_modulation(void);
		~Modulation() {}
};

#endif /* WIREDTO154_MODELS_MODULATION_H */
