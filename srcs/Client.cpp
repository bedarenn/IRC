#include "Client.hpp"

Client::Client() {}
Client::~Client() {}
Client::Client(const Client& cpy) { *this = cpy; }

Client&	Client::operator=(const Client& cpy) {
	if (this == &cpy)
		return (*this);
	_fds = cpy._fds;
	return (*this);
}

void	Client::add_new(const w_fd& socket) {
	w_pollfd	fd;

	fd.fd = socket;
	fd.events = POLLIN;
	fd.revents = 0;
	_fds.push_back(fd);
}
void	Client::add_new(const w_pollfd& fd) {
	_fds.push_back(fd);
}

void	Client::event() {
	ERR_INT(poll(_fds.data(), _fds.size(), 0), 0, "poll");

	if (_fds[0].revents)
		connect();
	for (w_vect_pollfd::iterator it = _fds.begin() + 1; it < _fds.end(); it++)
		if (it->revents) read(it);
}

void	Client::connect() {
	w_sockaddr_in	adr;
	w_socklen		len = sizeof(w_sockaddr_in);
	w_fd	client = accept(_fds[0].fd, (w_sockaddr *)&adr, &len);

	if (client > 2) {
		add_new(client);
		std::cout << SRV_NEW_CLIENT(client) << std::endl;
	}
	else
		std::cerr << SRV_ERROR_ACCEPT(client) << std::endl; 
}

void	Client::read(w_vect_pollfd::iterator& poll) {
	ssize_t	size;

	size = recv(poll->fd, &buff, BUFFER_SIZE, 0);
	if (size < 0) {
		std::cerr << SRV_ERROR_RECV << std::endl;
	}
	if (size == 0) {
		buff[0] = '\0';
		_fds.erase(poll);
	}
	buff[size] = '\0';
	std::cout << poll->fd << ": " << buff << std::flush;
}

void	Client::write(std::string& str,
		const w_vect_pollfd& grp,
		const w_pollfd& clt, const w_pollfd& srv) {
	for (w_vect_pollfd::const_iterator it = grp.begin(); it < grp.end(); it++) {
		if (it->fd != clt.fd && it->fd != srv.fd) {
			send(it->fd, str.c_str(), str.size(), 0);
		}
	}
}

w_pollfd	set_pollfd(int fd, short int event, short int revent) {
	w_pollfd	pollfd;

	pollfd.fd = fd;
	pollfd.events = event;
	pollfd.revents = revent;
	return (pollfd);
}
