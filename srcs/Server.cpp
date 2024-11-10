#include <sstream>

#include <cstring>
#include <cstdlib>
#include <errno.h>

#include "Server.hpp"

Server::Server(const char *port) {
	t_rlimit	rlp;

	if (getrlimit(RLIMIT_NOFILE, &rlp) == -1)
		throw (ErrorInt("getrlimit"));
	_maxfd = rlp.rlim_cur;
	_fds = new FD[_maxfd];
	_port = atoi(port);
}
Server::~Server() {}
Server::Server(const Server& other) {
	*this = other;
}

Server&	Server::operator=(const Server& other) {
	if (this == &other)
		return (*this);
	return (*this);
}

Server::ErrorInt::ErrorInt(const std::string& str) {
	std::ostringstream	oss;

	oss << str << " error (" << __FILE__ << ", " << __LINE__ << "): " << strerror(errno);
	msg = oss.str();
}
Server::ErrorInt::~ErrorInt() throw() {}
const char	*Server::ErrorInt::what() const throw() {
	return (msg.c_str());
}

Server::ErrorVoid::ErrorVoid(const std::string& str) {
	std::ostringstream	oss;

	oss << str << " error (" << __FILE__ << ", " << __LINE__ << "): " << strerror(errno);
	msg = oss.str();
}
Server::ErrorVoid::~ErrorVoid() throw() {}
const char	*Server::ErrorVoid::what() const throw() {
	return (msg.c_str());
}
