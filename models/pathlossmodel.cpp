#include "pathlossmodel.hpp"

#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

PathLossModel::pathloss_map PathLossModel::available_pathloss;

/* parse extra command line arguments
  (does not actually parse any argument if not overloaded) */
void PathLossModel::parse_arguments(std::vector<std::string> options) {

	try {
		po::options_description desc("Path loss option options "
									 "(this Path Loss model does"
									 " not accept any arguments)");
		desc.add_options()
			("pathloss-help", "produce this help message")
		;

		po::variables_map vm;
		po::store(po::command_line_parser(options).
				  options(desc).run(), vm);
		po::notify(vm);

		if (vm.count("pathloss-help")) {
			std::cout << "Usage: options_description [options]\n";
			std::cout << desc;
			exit(EXIT_SUCCESS);
		}
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << "\n";
		exit(EXIT_SUCCESS);
	}
	return;
}

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

double LogNormalShadowing::compute_SINR(Node<>::node_ptr sender,
                                        Node<>::node_ptr receiver) throw(ModelException)  {
	float distance = sender->distance(* receiver.get());

    if (distance < this->d0) // the model is not valid within this distance
        throw ModelException("distance between nodes is below distance 0 (d0), "
                             "Log-Normal Shadowing model does not work");

    /* Pt - PL_d0 - 10 * eta * log10(d/d0) - X_sigma - Pn */
	return sender->get_txPower() - this->path_loss_d0 - 10 * this->path_loss * log10(distance / this->d0) - X_sigma() - this->background_noise;
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
