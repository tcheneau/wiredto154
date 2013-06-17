#ifndef WIREDTO154_MODELS_NODE_H_
#define WIREDTO154_MODELS_NODE_H_

#include "coordinate.hpp"

#include <string>

class Node {
		std::string name;
		Coordinate * coordinate;
		float txPower;
		float rxSensitivity;
	public:
		Node(std::string name, Coordinate * coordinate, float txPower, float rxSensitivity);
};

#endif /* WIREDTO154_MODELS_NODE_H_ */
