#include "modulation.hpp"

Modulation::modulation_map Modulation::available_modulations;

std::vector<std::string> Modulation::list_available_modulation(void) {
	std::vector<std::string> modulations;

	for(modulation_map::iterator i = available_modulations.begin(); i != available_modulations.end(); ++i)
		modulations.push_back(i->first + ": " + i->second->get_description());

	return modulations;
}

void Modulation::register_modulation(modulation_ptr modulation) {
	if (available_modulations.count(modulation->get_name()))
		throw ModulationAlreadyRegistered();
	available_modulations[modulation->get_name()] = modulation;
}

Modulation::modulation_ptr Modulation::lookup_modulation(std::string & modname) {
	if (!available_modulations.count(modname))
		throw ModulationNotFoundException();
	return available_modulations[modname];
}
