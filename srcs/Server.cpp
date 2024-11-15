#include "Server.hpp"

#include <fcntl.h>
#include <unistd.h>

Server::Server() {}
Server::Server(w_port port, w_pass pass) : _port(port), _pass(pass) {}
Server::~Server() {
	for (w_vect_pollfd::iterator it = _fds.begin(); it < _fds.end(); it++) {
		close(it->fd);
	}
}
Server::Server(const Server &cpy) { *this = cpy; }

Server &Server::operator=(const Server &cpy) {
	if (this == &cpy)
		return (*this);
	_port = cpy._port;
	_pass = cpy._pass;
	_fds = cpy._fds;
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

	new_fd(fd);
	std::cout << "server init" << std::endl;
}

void	Server::run() {
	g_loop = 1;
	while (g_loop) {
		event();
	}
}

void	Server::new_fd(const w_fd& socket) {
	w_pollfd	fd;

	fd.fd = socket;
	fd.events = POLLIN;
	fd.revents = 0;
	_fds.push_back(fd);
	recv(socket, &buff, BUFFER_SIZE, 0);
	std::cout << socket << ": " << buff << std::flush;
}
void	Server::new_fd(const w_pollfd& fd) {
	_fds.push_back(fd);
}
void	Server::new_client(const std::string& name, const std::string& nickname, const w_fd& fd) {
	new_client(Client(name, nickname, fd));
}
void	Server::new_client(const Client& client) {
	_client.insert(w_pair_Client(client.get_fd(), client));
}

void	Server::event() {
	if (poll(_fds.data(), _fds.size(), 0) < 0) {
		g_loop = 0;
		return ;
	}

	if (_fds[0].revents)
		connect();
	for (w_vect_pollfd::iterator it = _fds.begin() + 1; it < _fds.end(); it++)
		if (it->revents) read(it);
}

void	Server::connect() {
	w_sockaddr_in	adr;
	w_socklen		len = sizeof(w_sockaddr_in);
	w_fd			client = accept(_fds[0].fd, (w_sockaddr *)&adr, &len);

	if (client > 2) {
		std::cout << SRV_NEW_CLIENT(client) << std::endl;
		new_fd(client);
	}
	else
		std::cerr << SRV_ERROR_ACCEPT(client) << std::endl; 
}

void	Server::rm__client(w_fd fd) {
	{
		w_map_Client::iterator	it = _client.find(fd);
		if (it != _client.end())
			_client.erase(it);
	}
	for (w_channel::iterator it = _channel.begin(); it != _channel.end(); it++) {
		it->second.rm__client(fd);
	}
	
}

void	Server::read(w_vect_pollfd::iterator& poll) {
	ssize_t	size;

	size = recv(poll->fd, &buff, BUFFER_SIZE, 0);
	if (size < 0) {
		std::cerr << SRV_ERROR_RECV << std::endl;
	}
	else if (size == 0) {
		buff[0] = '\0';
		close(poll->fd);
		_fds.erase(poll);
	}
	else {
		buff[size] = '\0';
		std::cout << poll->fd << ": " << buff << std::flush;
	}
}

w_port		Server::get_port() const	{ return(_port); }
w_pass		Server::get_pass() const	{ return(_pass); }

w_pollfd	set_pollfd(int fd, short int event, short int revent) {
	w_pollfd	pollfd;

	pollfd.fd = fd;
	pollfd.events = event;
	pollfd.revents = revent;
	return (pollfd);
}

void	Server::treatement(int fd, char *buff){
	std::stringstream ss(buff);
	std::string data;

	getline(ss, data, ' ');
	std::map<std::string, void(*)(std::string, int)>::iterator it;
	for(it = _cmd.begin(); it != _cmd.end(); it++){
		if(data == it->first){
			std::string arg(buff);
			it->second(arg, fd);
			return;
		}
		else if(it == _cmd.end())
			std::cout << "command not find" << std::endl;
	}
}
