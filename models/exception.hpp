#ifndef WIREDTO154_MODELS_EXCEPTION_H
#define WIREDTO154_MODELS_EXCEPTION_H

#include <exception>
#include <string>

class ModelException: public std::exception {
	std::string _what;
	public:
	  ModelException(const std::string & reason) { this->_what = reason; }
	  ~ModelException() throw() {}
	  const char * what(void) const throw() { return const_cast<const char * >(this->_what.c_str()); }
};

#endif /* WIREDTO154_MODELS_EXCEPTION_H */
