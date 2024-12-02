#include "ErrorInt.hpp"

# include <sstream>
# include <errno.h>
# include <cstring>

ErrorInt::ErrorInt(const std::string& str, const std::string& file, const int& line) {
	std::ostringstream	oss;

	oss << ERR_INT_MSG(str, file, line);
	msg = oss.str();
}
ErrorInt::~ErrorInt() throw() {}

const char	*ErrorInt::what() const throw() { return (msg.c_str()); }

void	ErrorInt::error_int
		(const int& f, const int& max,
		const std::string& str, const std::string& file, const int& line) {
	if (f < max)
		throw (ErrorInt(str, file, line));
}
