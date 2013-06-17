#include <cmath>
#include "coordinate.hpp"


Coordinate2D::Coordinate2D(void) {
	x = 0; y = 0;
}

float Coordinate2D::distance(Coordinate * b) {
	Coordinate2D * other = dynamic_cast<Coordinate2D *>(b);
	return sqrt(pow(this->x - other->x, 2) + pow(this->y - other->y, 2));
}
