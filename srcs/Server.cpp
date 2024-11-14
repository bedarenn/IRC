#include "Server.hpp"

#include <fcntl.h>

#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>

Server::Server() {}
Server::Server(w_port port, w_pass pass) : _port(port), _pass(pass) {}
Server::~Server() {}
Server::Server(const Server &cpy) { *this = cpy; }

Server &Server::operator=(const Server &cpy) {
	if (this == &cpy)
		return (*this);
	_port = cpy._port;
	_pass = cpy._pass;
	_client = cpy._client;
	return (*this);
}

void	Server::init_server() {
	w_pollfd		fd;
	w_sockaddr_in	adr;

	fd = set_pollfd(socket(AF_INET, SOCK_STREAM, 0), POLLIN, 0);
	ERR_INT(fd.fd, 0, "socket");

	fcntl(fd.fd, F_SETFL, O_NONBLOCK);
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = INADDR_ANY;
	adr.sin_port = htons(_port);

	ERR_INT(bind(fd.fd, (w_sockaddr *)&adr, sizeof(adr)), 0, "bind");
	ERR_INT(listen(fd.fd, 5), 0, "listen");

	_client.add_new(fd);
	std::cout << "server init" << std::endl;
}

void	Server::run() {
	g_loop = 1;
	while (g_loop) {
		event();
	}
}

void	Server::event() {
	_client.event();
}

w_port		Server::get_port() const	{ return(_port); }
w_pass		Server::get_pass() const	{ return(_pass); }
