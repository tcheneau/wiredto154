#include "pathlossmodel.hpp"

#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

/****************************************/
/**** Generic (non fonctional) model ****/
/****************************************/
PathLossModel::pathloss_map PathLossModel::available_pathloss;

/* parse extra command line arguments
  (does not actually parse any argument if not overloaded) */
void PathLossModel::parse_arguments(std::vector<std::string> options) {
	try {
		po::options_description desc("Path loss option options "
									 "(this Path Loss model does"
									 " not accept any arguments)");
		desc.add_options()
			("help-pathloss", "produce this help message")
		;

		po::variables_map vm;
		po::store(po::command_line_parser(options).
				  options(desc).run(), vm);
		po::notify(vm);

		if (vm.count("help-pathloss")) {
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

/******************************/
/**** Log-Normal Shadowing ****/
/******************************/
const std::string & LogNormalShadowing::get_model_name(void) {
	static std::string name("Log-Normal Shadowing");
	return name;
}

const std::string LogNormalShadowing::print_parameters(void) {
	std::stringstream params;
	params << "distance 0: " << this->d0 << "; "
		   << "path loss (eta): " << this->path_loss << "; "
		   << "shadowing deviation (sigma): " << this->shadowing_deviation << "; "
		   << "path loss at distance 0: " << this->path_loss_d0 << "; "
		   << "background noise: " << this->background_noise;
	return params.str();
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
									   PathLossModel("Log-Normal Shadowing") {
	this->simplistic = false;
	this->name = name;
	this->description = description;
	this->d0 = d0;
	this->path_loss = path_loss;
	this->shadowing_deviation = shadowing_deviation;
	this->path_loss_d0 = path_loss_d0;
	this->background_noise = background_noise;
}


/********************/
/**** Unit Disc ****/
/*******************/
const std::string &UnitDisc::get_model_name() {
	static std::string name("Unitary Disc");
	return name;
}

const std::string UnitDisc::print_parameters(void) {
	std::stringstream params;
	params << "good node radius: " << this->good_radius << "; "
		   << "damaged frame radius: " << this->damaged_radius;
	return params.str();
}

void UnitDisc::parse_arguments(std::vector<std::string> options) {
	try {
		po::options_description desc("Path loss option options "
									 "(this Path Loss model does"
									 " not accept any arguments)");
		desc.add_options()
			("help-pathloss", "produce this help message")
			("good-radius",
			 po::value<float>(&this->good_radius)->default_value(10),
			 "radius within which all packets are received")
			("damaged-radius",
			 po::value<float>(&this->damaged_radius)->default_value(20),
			 "radius within which all packets are received but damaged")
		;

		po::variables_map vm;
		po::store(po::command_line_parser(options).
				  options(desc).run(), vm);
		po::notify(vm);

		if (vm.count("help-pathloss")) {
			std::cout << "Usage: options_description [options]\n";
			std::cout << desc;
			exit(EXIT_SUCCESS);
		}

		if (this->good_radius < 0) {
			std::cout << "value passed to --good-radius must be positive"
					  << std::endl;
			exit(EXIT_FAILURE);
		}

		if (this->damaged_radius < 0) {
			std::cout << "value passed to --damaged-radius must be positive"
					  << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << "\n";
		exit(EXIT_SUCCESS);
	}
	return;
}

reception_type UnitDisc::receivePacket(Node<>::node_ptr sender, Node<>::node_ptr receiver, int msg_size) throw(ModelException) {
	float distance = sender->distance(* receiver.get());
	if (distance <= this->good_radius) {
		return PACKET_OK;
	} else if (distance <= this->damaged_radius) {
		return PACKET_CORRUPTED;
	} else {
		return PACKET_NOT_RECEIVED;
	}
}

