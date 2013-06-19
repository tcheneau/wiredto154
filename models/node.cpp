#include "node.hpp"

template<typename Coord>
Node<Coord>::Node(const std::string & name, const Coord & coordinate, float txPower, float rxSensitivity) {
	this->name = name;
	this->coordinate = coordinate;
	this->txPower = txPower;
	this->rxSensitivity = rxSensitivity;
}
