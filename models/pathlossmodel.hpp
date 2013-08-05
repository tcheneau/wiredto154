#ifndef WIREDTO154_MODELS_PATHLOSSMODEL_H
#define WIREDTO154_MODELS_PATHLOSSMODEL_H

/* this file defines the various path loss models */
#include "exception.hpp"
#include "modulation.hpp"
#include "node.hpp"
#include "types.hpp"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <exception>
#include <map>
#include <string>
#include <vector>


class PathLossModel {
	public:
		typedef boost::shared_ptr<PathLossModel> pathloss_ptr;
		typedef std::map<std::string, pathloss_ptr> pathloss_map;
		struct PathLossNotFoundException: std::exception {};
		struct PathLossAlreadyExistsException: std::exception {};
	private:
		static pathloss_map available_pathloss;
		const std::string model_name;
	protected:
		std::string name;
		std::string description;
		PathLossModel(const std::string & name): model_name(name) {}
	public:
		/* tell if the model is simplistic (and return a reception_type directly)
		 * or advanced (and can only compute a SINR) */
		bool simplistic;
		virtual const std::string & get_model_name(void) = 0;
        virtual double compute_SINR(Node<>::node_ptr sender, Node<>::node_ptr receiver) throw(ModelException) = 0;
		virtual reception_type receivePacket(Node<>::node_ptr sender, Node<>::node_ptr receiver, int msg_size) throw(ModelException) = 0;
		virtual const std::string& get_model_name(void) const { return model_name; }
		virtual const std::string& get_name(void) const { return name; }
		virtual const std::string& get_description(void) const { return description; }
		virtual void parse_arguments(std::vector<std::string> options);
		static void register_model(pathloss_ptr model);
		static pathloss_ptr lookup_pathloss(const std::string & name);
		static std::vector<std::string> list_available_model(void);
};


/* Log-Normal Shadowing, for more information, you can refer to p.104 of
 * "Wireless communication Principles & Practice" (Theodore S. Rappaport) */
class LogNormalShadowing: public PathLossModel {
	double d0; /* distance d0 */
	double path_loss; /* (Greek letter eta) */
	double shadowing_deviation; /* (Greek letter sigma) */
	double path_loss_d0; /* Path loss at distance 0 */
	double background_noise;

	typedef boost::normal_distribution<double> normal_dist;
	normal_dist randvar;
	boost::variate_generator<boost::mt19937 &, normal_dist> X_sigma;

	public:
	LogNormalShadowing(const std::string& name,
					   const std::string& description,
					   double d0,
					   double path_loss,
					   double shadowing_deviation,
					   double path_loss_d0,
					   double background_noise,
					   boost::mt19937 & randomness);
	virtual const std::string & get_model_name(void);
    virtual double compute_SINR(Node<>::node_ptr sender, Node<>::node_ptr receiver) throw(ModelException);
	reception_type receivePacket(Node<>::node_ptr sender, Node<>::node_ptr receiver, int msg_size) throw(ModelException) {
		throw ModelException("advanced models cannot use receivePacket()");
	};
};

/* Unit Disc path loss model */
class UnitDisc: public PathLossModel {
	float good_radius; /* radius within which transmission is always good */
	float damaged_radius; /* within the area comprized between the good_radius
							  and damaged_radius, nodes receive damaged frames */

	public:
	UnitDisc(const std::string& name,
			 const std::string& description):
			 PathLossModel("Unit Discs") {
		this->simplistic = true;
		this->name = name;
		this->description = description;
	}
	const std::string & get_model_name(void);
	void parse_arguments(std::vector<std::string> options);
	double compute_SINR(Node<>::node_ptr sender, Node<>::node_ptr receiver) throw(ModelException) {
		throw ModelException("simplistic models cannot use compute_SINR()");
	};
	reception_type receivePacket(Node<>::node_ptr sender, Node<>::node_ptr receiver, int msg_size) throw(ModelException) ;
};

#endif /* WIREDTO154_MODELS_PATHLOSSMODEL_H */
