#include "Server.hpp"

#include <fcntl.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>

#include <netinet/in.h>
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
	// _server_fd = cpy._server_fd;
	// _fds = cpy._fds;
	return (*this);
}

void	Server::init_server() {
	w_sockaddr_in	adr;

	_fd = set_pollfd(socket(AF_INET, SOCK_STREAM, 0), POLLIN, 0);
	ERR_INT(_fd.fd, 0, "socket");

	fcntl(_fd.fd, F_SETFL, O_NONBLOCK);
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = INADDR_ANY;
	adr.sin_port = htons(_port);

	ERR_INT(bind(_fd.fd, (w_sockaddr *)&adr, sizeof(adr)), 0, "bind");
	ERR_INT(listen(_fd.fd, 5), 0, "listen");

	std::cout << "server init" << std::endl;
}

void	Server::run() {
	g_loop = 1;
	while (g_loop) {
		event();
	}
}

void	Server::event() {
	ERR_INT(poll(&_fd, 1, 0), 0, "poll");

	if (_fd.revents)
		connect();
	_client.event();
}

void	Server::connect() {
	w_sockaddr_in	adr;
	w_socklen		len = sizeof(w_sockaddr_in);
	w_fd	client = accept(_fd.fd, (w_sockaddr *)&adr, &len);

	if (client > 2) {
		add_new(client);
		std::cout << SRV_NEW_CLIENT(client) << std::endl;
	}
	else
		std::cerr << SRV_ERROR_ACCEPT(client) << std::endl; 
}

void	Server::add_new(int socket) {
	_client.add_new(socket);
}

w_port		Server::get_port() const	{ return(_port); }
w_pass		Server::get_pass() const	{ return(_pass); }
w_pollfd	Server::get_fd() const		{ return(_fd); }
