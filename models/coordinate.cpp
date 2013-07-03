#include "coordinate.hpp"

#include <boost/lexical_cast.hpp>
#include <cmath>
#include <iostream>

std::ostream & Coordinate::dump(std::ostream & os) const {
	std::cout << "Coordinate dump() has been called" << std::endl;
	return os;
}

Coordinate2D::Coordinate2D(void) { this->x = 0; this->y = 0;}
Coordinate2D::Coordinate2D(float x, float y) { this->x = x; this->y = y;}

std::ostream & Coordinate2D::dump(std::ostream & os) const {
	return os << "x: " << boost::lexical_cast<std::string>(x) << std::endl
			  << "y: " << boost::lexical_cast<std::string>(y);
}

std::ostream & operator<<(std::ostream & os, const Coordinate & c) {
	return c.dump(os);
}


float Coordinate2D::distance(Coordinate * b) {
	Coordinate2D * other = dynamic_cast<Coordinate2D *>(b);
	return sqrt(pow(this->x - other->x, 2) + pow(this->y - other->y, 2));
}

