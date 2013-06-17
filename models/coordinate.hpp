#ifndef WIRED154_MODELS_COORDINATE_H_
#define WIRED154_MODELS_COORDINATE_H_

class Coordinate {
	public:
		virtual float distance(Coordinate * b) = 0;
};


class Coordinate2D: public Coordinate {
	float x;
	float y;
	public:
		Coordinate2D();
		Coordinate2D(float x, float y) { this->x = x; this->y = y;};
		virtual float distance(Coordinate * b);
};

#endif /* WIRED154_MODELS_COORDINATE_H_ */
