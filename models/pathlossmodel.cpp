#include "pathlossmodel.hpp"

PathLossModel::pathloss_map PathLossModel::available_pathloss;

void PathLossModel::register_model(pathloss_ptr model) {
	if (available_pathloss.count(model->get_name()))
		throw PathLossAlreadyExistsException();
	available_pathloss[model->get_name()] = model;
}

PathLossModel::pathloss_ptr PathLossModel::lookup_pathloss(const std::string & name) {
	if (!available_pathloss.count(name))
		throw PathLossNotFoundException();
	return available_pathloss[name];
}

std::vector<std::string> PathLossModel::list_available_model(void) {
	std::vector<std::string> pathloss;

	for(pathloss_map::iterator i = available_pathloss.begin(); i != available_pathloss.end(); ++i)
		pathloss.push_back(std::string("- name: ") + i->first + "\n  base model: " + i->second->get_model_name() + "\n  details: " + i->second->get_description());

	return pathloss;
}

const std::string & LogNormalShadowing::get_model_name(void) {
	static std::string name("Log-Normal Shadowing");
	return name;
}

double LogNormalShadowing::compute_SINR(Node<> & sender, Node<> & receiver) throw(ModelException) {
	float distance = sender.distance(receiver);

	if (distance < this->d0) // the model is not valid within this distance
		throw ModelException("distance between nodes is below distance 0 (d0), "
							 "Log-Normal Shadowing model does not work");

	/* Pt - PL_d0 - 10 * eta * log10(d/d0) - X_sigma - Pn */
	return sender.get_txPower() - this->path_loss_d0 - 10 * this->path_loss * log10(distance / this->d0) - X_sigma() - this->background_noise;
}


LogNormalShadowing::LogNormalShadowing(const std::string& name,
									   const std::string& description,
									   double d0,
									   double path_loss,
									   double shadowing_deviation,
									   double path_loss_d0,
									   double background_noise,
									   boost::mt19937 & randomness):
									   randvar(0.0, shadowing_deviation),
									   X_sigma(randomness, randvar),
									   PathLossModel("Log-Normal Shadowing"){
	this->name = name;
	this->description = description;
	this->d0 = d0;
	this->path_loss = path_loss;
	this->shadowing_deviation = shadowing_deviation;
	this->path_loss_d0 = path_loss_d0;
	this->background_noise = background_noise;
}
