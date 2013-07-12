#ifndef WIREDTO154_MODELS_NODE_H_
#define WIREDTO154_MODELS_NODE_H_

/* this file defines node objects, that is, the virtual node that will be
 * emulated and that maps to a physical node */

#include "coordinate.hpp"


#include <boost/shared_ptr.hpp>
#include <iostream>
#include <list>
#include <string>

template <typename Coord> class Node;

template <typename Coord>
std::ostream & operator<< (std::ostream & os, const Node<Coord> & node);

template<typename Coord=Coordinate2D>
class Node {
		const std::string description;
		const std::string type;
		Coord coordinate;
		int id;
		float txPower;
		float rxSensitivity;
	public:
		typedef boost::shared_ptr< Node<Coord> > node_ptr;
		Node(int id,
			 const Coord & coordinate,
			 float txPower,
			 float rxSensitivity,
			 const std::string & description,
			 const std::string & type);
		float distance(Node<Coord> & other) { return coordinate.distance(&other.coordinate); }
		float get_txPower(void) { return txPower; }
		float get_rxSensitivity(void) { return rxSensitivity; }
		int get_id(void) const { return id; }
		const std::string & get_description(void) const { return description; }
		const std::string & get_type(void) const { return type; }
		friend std::ostream & operator<< <> (std::ostream & os, const Node<Coord> & node);
};

template<typename Coord>
Node<Coord>::Node(int id,
				  const Coord & coordinate,
				  float txPower,
				  float rxSensitivity,
				  const std::string & node_description,
				  const std::string & node_type
				  )
: description(node_description), type(node_type)
{
	this->id = id;
	this->coordinate = coordinate;
	this->txPower = txPower;
	this->rxSensitivity = rxSensitivity;
}

template<typename Coord>
std::ostream & operator<< (std::ostream & os, const Node<Coord> & node) {
	os << "coordinate: " << std::endl << node.coordinate << std::endl
	   << "txPower: " << node.txPower << std::endl
	   << "rxSensitivity: " << node.rxSensitivity << std::endl
	   << "type: " << node.type << std::endl
	   << "description: " << node.description;
	return os;
}

#endif /* WIREDTO154_MODELS_NODE_H_ */
