#ifndef WIREDTO154_MODELS_PATHLOSSMODEL_H
#define WIREDTO154_MODELS_PATHLOSSMODEL_H

/* this file defines the various path loss models */
#include "exception.hpp"
#include "modulation.hpp"
#include "node.hpp"

#include <string>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>


class PathLossModel {
	std::string name;
	public:
	virtual const std::string & get_model_name(void) = 0;
	virtual bool receivePacket(Node<> & sender,
			Node<> & receiver,
			const std::string & msg,
			Modulation modscheme)=0;
	virtual double compute_SINR(Node<> & sender, Node<> & receiver) throw(ModelException) = 0;
	void init(void);
	void register_model(PathLossModel& model);
	void set_name(const std::string& name);
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
	LogNormalShadowing(double d0,
			double path_loss,
			double shadowing_deviation,
			double path_loss_d0,
			double background_noise,
			boost::mt19937 & randomness);
	LogNormalShadowing(const std::string& name,
			double d0,
			double path_loss,
			double shadowing_deviation,
			double path_loss_d0,
			double background_noise,
			boost::mt19937 & randomness);
	virtual const std::string & get_model_name(void);
	virtual double compute_SINR(Node<> & sender, Node<> & receiver) throw(ModelException);
};

#endif /* WIREDTO154_MODELS_PATHLOSSMODEL_H */
