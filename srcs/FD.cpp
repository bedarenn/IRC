#include "FD.hpp"

FD::FD() : _type(0) {}
FD::~FD() {}
FD::FD(const FD& other) {
	*this = other;
}

FD&	FD::operator=(const FD& other) {
	if (this == &other)
		return (*this);
	return (*this);
}
