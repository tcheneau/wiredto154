#ifndef WIREDTO154_MODELS_TOOLS_H_
#define WIREDTO154_MODELS_TOOLS_H_

#include <cmath>

template<typename T>
inline T log_to_linear(T x) {
	return std::pow(10, x/10);
}
#endif
