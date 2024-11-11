#include "Server.hpp"

#include <fcntl.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>

#include <netinet/in.h>
#include <arpa/inet.h>

Server::Server() {}
Server::Server(w_port port, w_pass pass) : _port(port), _pass(pass) {}
Server::Server(const Server &cpy) { *this = cpy; }
Server::~Server() {}

Server &Server::operator=(const Server &cpy) {
	if (this == &cpy)
		return (*this);
	_port = cpy._port;
	_pass = cpy._pass;
	// _fds = cpy._fds;
	return (*this);
}

void		Server::add_new(int socket) {
	pollfd fd;

	fd.fd = socket;
	fd.events = POLLIN;
	fd.revents = 0;
	_fds.push_back(fd);
}

void	Server::init_server() {
	struct sockaddr_in adr;
	pollfd	fd;

	fd.fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd.fd < 0){
		std::cerr << "error: socket" << std::endl;
		return ;
	}
	_server_fd = fd.fd;
	fcntl(fd.fd, F_SETFL, O_NONBLOCK);
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = INADDR_ANY;
	adr.sin_port = htons(_port);
	if(bind(fd.fd, (struct sockaddr*)&adr, sizeof(adr)) < 0)
		std::cerr << "error: bind" << std::endl;
	if(listen(fd.fd, 5) < 0)
		std::cerr << "error: listen" << std::endl;
	fd.events = POLLIN;
	fd.revents = 0;
	_fds.push_back(fd);
}

void	Server::run(){
	while(1){
		if(poll(_fds.data(), _fds.size(), 0) < 0){
			std::cerr << "error: poll" << std::endl;
			break;
		}
		for(size_t i = 0; i < _fds.size(); i++){
			if(_fds[i].revents && _fds[i].fd == _server_fd)
				connect();
			else if(_fds[i].revents && _fds[i].fd != _server_fd)
				received_data(_fds[i].fd);
		}
		clear_fd();
	}
}

void	Server::connect(){
	struct sockaddr_in adr;
	socklen_t len =  sizeof(adr);
	int	client = accept(_server_fd, (struct sockaddr*)&adr, &len);
	std::cout << "client fd = " << client << std::endl;
	if(client){
		add_new(client);
		std::cout << "new client connected" << std::endl;
	}
	else
		std::cerr << "error: accept" << std::endl; 
}

void		Server::received_data(size_t fd){
	char	buff[BUFFSIZE];
	if(recv(fd, &buff, sizeof(BUFFSIZE), 0) < 0){
		std::cerr << "error: recv" << std::endl;
		return ;
	}
	buff[BUFFSIZE] = '\0';
	std::cout << buff << std::endl;
}

void	Server::clear_fd(){
	for(size_t i = 0; i < _fds.size(); i++){
		_fds[i].revents = 0;
	}
}

w_port	Server::get_port() const		{ return(_port); }
w_pass	Server::get_pass() const		{ return(_pass); }
int		Server::get_fd_serv() const		{ return(_server_fd); }

void	Server::set_port(w_port port)	{ _port = port; }
void	Server::set_pass(w_pass pass)	{ _pass = pass; }
