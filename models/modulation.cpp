#include "modulation.hpp"

const std::string Modulation::name = "Unknown Modulation (not defined by the model)";
const std::string Modulation::description = "";

std::map<std::string, Modulation::modulation_ptr> Modulation::available_modulations;

std::vector<std::string> Modulation::list_available_modulation(void) {
	std::vector<std::string> modulations;

	for(std::map<std::string, modulation_ptr>::iterator i = available_modulations.begin(); i != available_modulations.end(); ++i)
		modulations.push_back(i->first + ": " + i->second->get_description());

	return modulations;
}

Modulation::modulation_ptr Modulation::lookup_modulation(std::string & modname) {
	if (!available_modulations.count(modname))
		throw ModulationNotFoundException();
	return available_modulations[modname];
}
