#include "node.hpp"

Node::Node(std::string name, Coordinate * coordinate, float txPower, float rxSensitivity) {
	this->name = name;
	this->coordinate = coordinate;
	this->txPower = txPower;
	this->rxSensitivity = rxSensitivity;
}
