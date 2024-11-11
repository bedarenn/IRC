#pragma once

# include <exception>

class	Server::ErrorInt : public std::exception {
public:
	ErrorInt(const std::string& str) {
		std::ostringstream	oss;

		oss << str << " error (" << __FILE__ << ", " << __LINE__ << "): " << strerror(errno);
		msg = oss.str();
	}
	~ErrorInt() throw() {}

	const char	*what() const throw() { return (msg.c_str()); }
private:
	std::string	msg;
};
