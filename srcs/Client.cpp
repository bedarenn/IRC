#include "Client.hpp"

#include <sys/socket.h>

Client::Client() {}
Client::~Client() {}
Client::Client(const Client& cpy) { *this = cpy; }

Client&	Client::operator=(const Client& cpy) {
	if (this == &cpy)
		return (*this);
	_fds = cpy._fds;
	return (*this);
}

void	Client::add_new(int socket) {
	w_pollfd	fd;

	fd.fd = socket;
	fd.events = POLLIN;
	fd.revents = 0;
	_fds.push_back(fd);
}
void	Client::event() {
	ERR_INT(poll(_fds.data(), _fds.size(), 0), 0, "poll");

	for (w_vect_pollfd::iterator it = _fds.begin(); it < _fds.end(); it++) {
		if (it->revents) {
			read(it);
		}
	}
}

void	Client::read(w_vect_pollfd::iterator& poll) {
	ssize_t	size;

	size = recv(poll->fd, &buff, BUFFER_SIZE, 0);
	if (size < 0) {
		std::cerr << SRV_ERROR_RECV << std::endl;
	}
	else if (size == 0) {
		buff[0] = '\0';
		_fds.erase(poll);
	}
	else {
		buff[size] = '\0';
		std::cout << poll->fd << ": " << buff << std::flush;
	}
}

w_pollfd	set_pollfd(int fd, short int event, short int revent) {
	w_pollfd	pollfd;

	pollfd.fd = fd;
	pollfd.events = event;
	pollfd.revents = revent;
	return (pollfd);
}
