#ifndef WIREDTO154_MODELS_NODE_H_
#define WIREDTO154_MODELS_NODE_H_

/* this file defines node objects, that is, the virtual node that will be
 * emulated and that maps to a physical node */

#include "coordinate.hpp"


#include <iostream>
#include <string>

template <typename Coord> class Node;

template <typename Coord>
std::ostream & operator<< (std::ostream & os, const Node<Coord> & node);

template<typename Coord=Coordinate2D>
class Node {
		const std::string & name;
		Coord coordinate;
		float txPower;
		float rxSensitivity;
	public:
		Node(const std::string & name, const Coord & coordinate, float txPower, float rxSensitivity);
		float distance(Node<Coord> & other) { return coordinate.distance(&other.coordinate); }
		float get_txPower(void) { return txPower; }
		float get_rxSensitivity(void) { return rxSensitivity; }
		const std::string & get_name(void) { return name; }
		friend std::ostream & operator<< <> (std::ostream & os, const Node<Coord> & node);
};

template<typename Coord>
Node<Coord>::Node(const std::string & node_name, const Coord & coordinate, float txPower, float rxSensitivity):
	name(node_name) {
	this->coordinate = coordinate;
	this->txPower = txPower;
	this->rxSensitivity = rxSensitivity;
}

template<typename Coord>
std::ostream & operator<< (std::ostream & os, const Node<Coord> & node) {
	os << "name:" << node.name << std::endl
	   << "coordinate:" << std::endl << node.coordinate << std::endl
	   << "txPower:" << node.txPower << std::endl
	   << "rxSensitivity" << node.rxSensitivity;
	return os;
}

#endif /* WIREDTO154_MODELS_NODE_H_ */
