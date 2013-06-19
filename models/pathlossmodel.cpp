#include "pathlossmodel.hpp"

void PathLossModel::init(void) {
	// initialize a hash table that will store all the defined Path Loss Model
}

void PathLossModel::register_model(PathLossModel& model) {
	// add an entry to the hash table
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
									   double background_noise):
									   randvar(0.0, shadowing_deviation), X_sigma(Simulation::randomness, randvar) {
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
									   double background_noise):
									   randvar(0.0, shadowing_deviation), X_sigma(Simulation::randomness, randvar) {
	this->set_name(name);
	this->d0 = d0;
	this->path_loss = path_loss;
	this->shadowing_deviation = shadowing_deviation;
	this->path_loss_d0 = path_loss_d0;
	this->background_noise = background_noise;
}

bool LogNormalShadowing::receivePacket(Node<> & sender,
	   								   Node<> & receiver,
									   const std::string & msg,
									   Modulation modscheme) {
	// compute the SNR at the receiver
	// apply the BER model to determine if the packets can be received
	return true;
}
