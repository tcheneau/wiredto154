#ifndef WIREDTO154_MODELS_NODE_H_
#define WIREDTO154_MODELS_NODE_H_

/* this file defines node objects, that is, the virtual node that will be
 * emulated and that maps to a physical node */

#include "coordinate.hpp"

#include <string>

template<typename Coord=Coordinate2D>
class Node {
		std::string & name;
		Coord coordinate;
		float txPower;
		float rxSensitivity;
	public:
		Node(const std::string & name, const Coord & coordinate, float txPower, float rxSensitivity);
		float distance(Node<Coord> & other) { return coordinate.distance(&other.coordinate); };
		float get_txPower(void) { return txPower; };
		float get_rxSensitivity(void) { return rxSensitivity; };
		const std::string & get_name(void) { return name; }
};

#endif /* WIREDTO154_MODELS_NODE_H_ */
