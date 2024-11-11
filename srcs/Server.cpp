#include <sstream>

#include <cstring>
#include <cstdlib>
#include <errno.h>

#include "Server.hpp"

Server::Server(const char *port) {
	w_rlimit	rlp;

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
