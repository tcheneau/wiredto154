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
		Coordinate2D();
		Coordinate2D(float x, float y);
		virtual float distance(Coordinate * b);
		virtual float get_x() const { return x; }
		virtual float get_y() const { return y; }
};

std::ostream & operator<<(std::ostream & os, const Coordinate & c);

#endif /* WIRED154_MODELS_COORDINATE_H_ */
