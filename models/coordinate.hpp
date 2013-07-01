#ifndef WIRED154_MODELS_COORDINATE_H_
#define WIRED154_MODELS_COORDINATE_H_

#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/base_object.hpp>
#include <iostream>

class Coordinate {
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive &ar, unsigned int version) { /* does nothing */ }
	public:
		virtual std::ostream & dump(std::ostream & os) const;
		virtual float distance(Coordinate * b) = 0;
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Coordinate);

class Coordinate2D: public Coordinate {
	private:
		friend class boost::serialization::access;
		float x;
		float y;
		template<class Archive>
		void serialize(Archive & ar, unsigned int version) {
			ar & boost::serialization::base_object<Coordinate>(*this);
			ar & x & y;
		}
	public:
		virtual std::ostream & dump(std::ostream & os) const;
		Coordinate2D() { this->x = 0; this->y = 0;};
		Coordinate2D(float x, float y) { this->x = x; this->y = y;};
		virtual float distance(Coordinate * b);
		virtual float get_x() const { return x; }
		virtual float get_y() const { return y; }
};

std::ostream & operator<<(std::ostream & os, const Coordinate & c);

#endif /* WIRED154_MODELS_COORDINATE_H_ */
