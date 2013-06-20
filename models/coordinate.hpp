#ifndef WIRED154_MODELS_COORDINATE_H_
#define WIRED154_MODELS_COORDINATE_H_

#include <iostream>

class Coordinate {
	public:
		virtual std::ostream & dump(std::ostream & os) const;
		virtual float distance(Coordinate * b) = 0;
};


class Coordinate2D: public Coordinate {
		float x;
		float y;
	public:
		virtual std::ostream & dump(std::ostream & os) const;
		Coordinate2D() { this->x = 0; this->y = 0;};
		Coordinate2D(float x, float y) { this->x = x; this->y = y;};
		virtual float distance(Coordinate * b);
};

std::ostream & operator<<(std::ostream & os, const Coordinate & c);

#endif /* WIRED154_MODELS_COORDINATE_H_ */
