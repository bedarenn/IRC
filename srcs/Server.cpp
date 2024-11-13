#include "Server.hpp"

#include <fcntl.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "ErrorInt.hpp"

Server::Server() {}
Server::Server(w_port port, w_pass pass) : _port(port), _pass(pass){} /*, _security(false)*/
Server::Server(const Server &cpy) { *this = cpy; }
Server::~Server() {}

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
	w_pollfd		fd;

	fd.fd = socket(AF_INET, SOCK_STREAM, 0);
	ERR_INT(fd.fd, 0, "socket");
	_server_fd = fd.fd;
	fcntl(fd.fd, F_SETFL, O_NONBLOCK);
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = INADDR_ANY;
	adr.sin_port = htons(_port);
	ERR_INT(bind(fd.fd, (w_sockaddr *)&adr, sizeof(adr)), 0, "bind");
	ERR_INT(listen(fd.fd, 5), 0, "listen");
	fd.events = POLLIN;
	fd.revents = 0;
	_fds.push_back(fd);
	std::cout << "server init" << std::endl;
}

void	Server::run() {
	g_loop = 1;
	while (g_loop) {
		ERR_INT(poll(_fds.data(), _fds.size(), 0), 0, "poll");
		for (w_vect_pollfd::iterator it = _fds.begin(); it < _fds.end(); it++){
			if (it->revents && it->fd == _server_fd)
				connect();
			else if (it->revents != 0 && it->fd != _server_fd){
				event(it);
			}
		}
	}
}

void	Server::connect() {
	w_sockaddr_in	adr;
	w_socklen		len = sizeof(w_sockaddr_in);
	w_fd	client = accept(_server_fd, (w_sockaddr *)&adr, &len);

	if (client > 2) {
		add_new(client);
		std::cout << SRV_NEW_CLIENT(client) << std::endl;
	}
	else
		std::cerr << SRV_ERROR_ACCEPT(client) << std::endl; 
}

void		Server::add_new(int socket) {
	w_pollfd	fd;

	fd.fd = socket;
	fd.events = POLLIN;
	fd.revents = 0;
	_fds.push_back(fd);
}

void		Server::event(w_vect_pollfd::iterator& poll) {
	ssize_t	size;

	size = recv(poll->fd, &buff, BUFFSIZE, 0);
	if (size < 0) {
		std::cerr << SRV_ERROR_RECV << std::endl;
		return ;
	}
	if (size == 0) {
		buff[0] = '\0';
		_fds.erase(poll);
	}	
	buff[size] = '\0';
	std::cout << buff << std::endl;
}

w_port	Server::get_port() const		{ return(_port); }
w_pass	Server::get_pass() const		{ return(_pass); }
int		Server::get_fd_serv() const		{ return(_server_fd); }

void	Server::set_port(w_port port)	{ _port = port; }
void	Server::set_pass(w_pass pass)	{ _pass = pass; }
