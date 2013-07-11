#include "pathlossmodel.hpp"

void PathLossModel::init(void) {
	// initialize a hash table that will store all the defined Path Loss Model
}

void PathLossModel::register_model(const std::string & name,
								   pathloss_ptr model) {
	if (available_pathloss.count(name))
		throw PathLossAlreadyExistsException();
	available_pathloss[name] = model;
}

void PathLossModel::set_name(const std::string& name) {
	this->name = name;
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


LogNormalShadowing::LogNormalShadowing(double d0,
									   double path_loss,
									   double shadowing_deviation,
									   double path_loss_d0,
									   double background_noise,
									   boost::mt19937 & randomness):
									   randvar(0.0, shadowing_deviation), X_sigma(randomness, randvar) {
	this->set_name("Log-Normal Shadowing -- unspecified");
	this->d0 = d0;
	this->path_loss = path_loss;
	this->shadowing_deviation = shadowing_deviation;
	this->path_loss_d0 = path_loss_d0;
	this->background_noise = background_noise;
}

LogNormalShadowing::LogNormalShadowing(const std::string& name,
									   double d0,
									   double path_loss,
									   double shadowing_deviation,
									   double path_loss_d0,
									   double background_noise,
									   boost::mt19937 & randomness):
									   randvar(0.0, shadowing_deviation), X_sigma(randomness, randvar) {
	this->set_name(name);
	this->d0 = d0;
	this->path_loss = path_loss;
	this->shadowing_deviation = shadowing_deviation;
	this->path_loss_d0 = path_loss_d0;
	this->background_noise = background_noise;
}
