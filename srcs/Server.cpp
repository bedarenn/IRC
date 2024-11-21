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
	_fds.push_back(fd);

	fcntl(fd.fd, F_SETFL, O_NONBLOCK);
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = INADDR_ANY;
	adr.sin_port = htons(_port);

	ERR_INT(bind(fd.fd, (w_sockaddr *)&adr, sizeof(adr)), 0, "bind");
	ERR_INT(listen(fd.fd, 5), 0, "listen");

	std::cout << "server init" << std::endl;
}

void	Server::run() {
	g_loop = 1;
	while (g_loop) {
		event();
	}
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
		Command(poll->fd, buff, this);
		//std::cout << poll->fd << ": " << buff << std::flush;
	}
}

void	Server::join(const w_fd& fd, const std::string& channel, const std::string& pass) {
	Client	client = get_client(fd);

	if (!join__channel(client, channel, pass))
		new_map_Channel(client, channel);
}
void	Server::invite(const w_fd& fd, const std::string& channel, const std::string& client) {
	Client	op = get_client(fd);

	w_map_Channel::iterator it_channel = _channel.find(channel);
	w_map_Client::iterator	it_client = get_client(client);

	it_channel->second.invite(op, it_client->second);
}
void	Server::kick(const w_fd& fd, const std::string& channel, const std::string& client) {
	Client	op = get_client(fd);

	w_map_Channel::iterator it_channel = _channel.find(channel);
	w_map_Client::iterator	it_client = get_client(client);

	it_channel->second.kick(op, it_client->second);
}
void	Server::topic(const w_fd& fd, const std::string& channel, const std::string& value) {
	Client	op = get_client(fd);

	w_map_Channel::iterator it_channel = _channel.find(channel);

	it_channel->second.topic(op, value);
}


void	Server::new_fd(const w_fd& socket) {
	w_pollfd	fd;

	fd.fd = socket;
	fd.events = POLLIN;
	fd.revents = 0;
	_fds.push_back(fd);
	recv(socket, &buff, BUFFER_SIZE, 0);
	Command(socket, buff, this);
}
void	Server::new_client(const std::string& name, const std::string& nickname, const w_fd& fd) {
	Client(name, nickname, fd).add_to_map(_client);
}
void	Server::rm__client(const Client& client) {
	client.rm__to_map(_client);
	for (w_map_Channel::iterator it = _channel.begin(); it != _channel.end(); it++) {
		it->second.rm__client(client);
	}
}


bool	Server::join__channel(const Client& client, const std::string& channel, const std::string& pass) {
	w_map_Channel::iterator	it = _channel.find(channel);

	if (it == _channel.end())
		return (false);
	it->second.join(client, pass);
	return (true);
}
bool	Server::leave_channel(const Client& client, const std::string& channel) {
	w_map_Channel::iterator	it = _channel.find(channel);

	if (it == _channel.end())
		return (false);
	it->second.rm__client(client);
	if (it->second.empty())
		_channel.erase(it);
	return (true);
}

void	Server::new_map_Channel(const Client& client, const std::string& channel) {
	_channel.insert(w_pair_channel(channel, Channel(client, channel)));
}

w_port		Server::get_port() const	{ return(_port); }
w_pass		Server::get_pass() const	{ return(_pass); }

const Client&	Server::get_client(w_fd fd) const {
	w_map_Client::const_iterator it = _client.find(fd);
	if (it == _client.end())
		throw (std::runtime_error("Client Unknown"));
	return (it->second);
}
w_map_Client::iterator	Server::get_client(std::string name) {
	w_map_Client::iterator it;
	for (it = _client.begin(); it != _client.end() && it->second.get_name() != name; it++) ;
	return (it);
}

w_pollfd	set_pollfd(int fd, short int event, short int revent) {
	w_pollfd	pollfd;

	pollfd.fd = fd;
	pollfd.events = event;
	pollfd.revents = revent;
	return (pollfd);
}
