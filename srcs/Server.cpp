#include "Server.hpp"

#include <cstring>
#include <fcntl.h>
#include <unistd.h>

Server::Server() {}
Server::Server(const std::string& name, w_port port, w_pass pass) : _name(name.substr(2, name.size() - 2)), _port(port), _pass(pass) {}
Server::~Server() {
	for (w_vect_pollfd::iterator it = _fds.begin(); it < _fds.end(); it++) {
		close(it->fd);
	}
}
Server::Server(const Server &cpy) { *this = cpy; }

Server &Server::operator=(const Server &cpy) {
	if (this == &cpy)
		return (*this);
	_name = cpy._name;
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

	std::cout << _name << ": server init" << std::endl;
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
	std::string str("");
	ssize_t		size;

	bzero(buff, BUFFER_SIZE);
	size = recv(poll->fd, &buff, BUFFER_SIZE, 0);
	buff[size] = '\0';
	if (size < 0)
		std::cerr << SRV_ERROR_RECV << std::endl;
	else if (size == 0) {
		close(poll->fd);
		_fds.erase(poll);	
	}
	else {
		str = std::string(str + buff);
		while (str.size() < 2 && str.at(str.size() - 2) != '\r' && str.at(str.size() - 2) != '\n') {
			bzero(buff, BUFFER_SIZE);
			size = recv(poll->fd, &buff, BUFFER_SIZE, 0);
			buff[size] = '\0';
			if (str.size())
				str = std::string(str + buff);
		}
		Command(poll->fd, str, this);
	}
}

void	Server::join(const w_fd& fd, const std::string& channel, const std::string& pass) {
	try {
		Client	client = get_client(fd);
		if (channel.empty()) {
			client.send_to_fd(W_ERR_NEEDMOREPARAMS(client, "JOIN", _name));
			return ;
		}
		if (!join__channel(client, channel, pass))
			new_map_Channel(client, channel);
	} catch (std::exception& err) {
		std::cerr << "catch: " << err.what() << std::endl;
		return ;
	}
}
void	Server::invite(const w_fd& fd, const std::string& channel, const std::string& client) {
	try {
		Client	op = get_client(fd);

		if (channel.empty() || client.empty()) {
			op.send_to_fd(W_ERR_NEEDMOREPARAMS(op, "INVITE", _name));
			return ;
		}

		w_map_Channel::iterator it_channel = _channel.find(channel);

		if (it_channel == _channel.end()) {
			op.send_to_fd(W_ERR_NOSUCHCHANNEL(op, "INVITE", _name));
			return ;
		}

		it_channel->second.invite(op, client);
	} catch (std::exception& err) {
		std::cerr << "catch: " << err.what() << std::endl;
		return ;
	}


}
void	Server::kick(const w_fd& fd, const std::string& channel, const std::string& client, const std::string& msg) {
	try {
		Client	op = get_client(fd);

		if (channel.empty() || client.empty()) {
			op.send_to_fd(W_ERR_NEEDMOREPARAMS(op, "KICK", _name));
			return ;
		}
		w_map_Channel::iterator it_channel = _channel.find(channel);

		if (it_channel == _channel.end()) {
			op.send_to_fd(W_ERR_NOTONCHANNEL(op, "KICK", _name));
			return ;
		}

		w_map_Client::iterator	it_client = get_client(client);

		it_channel->second.kick(op, it_client->second, msg);
	} catch (std::exception& err) {
		std::cerr << "catch: " << err.what() << std::endl;
		return ;
	}
}
void	Server::topic(const w_fd& fd, const std::string& channel, const std::string& value) {
	try {
		Client	op = get_client(fd);

		if (channel.empty() || value.empty()) {
			op.send_to_fd(W_ERR_NEEDMOREPARAMS(op, "TOPIC", _name));
			return ;
		}
		w_map_Channel::iterator it_channel = _channel.find(channel);
		if (it_channel == _channel.end()) {
			op.send_to_fd(W_ERR_NOSUCHCHANNEL(op, "TOPIC", _name));
			return ;
		}
		it_channel->second.topic(op, value);
	} catch (std::exception& err) {
		std::cerr << "catch: " << err.what() << std::endl;
		return ;
	}
}
void	Server::mode(const w_fd& fd, const std::string& channel, const std::string& md, const std::string& arg) {
	try {
		Client	op = get_client(fd);

		if (channel.empty()) {
			op.send_to_fd(W_ERR_NEEDMOREPARAMS(op, "MODE", _name));
			return ;
		}
		w_map_Channel::iterator it_channel = _channel.find(channel);
		if (it_channel == _channel.end()) {
			op.send_to_fd(W_ERR_NOSUCHCHANNEL(op, "MODE", _name));
			return ;
		}
		it_channel->second.mode(op, md, arg);
	} catch (std::exception& err) {
		std::cerr << "catch: " << err.what() << std::endl;
		return ;
	}
}
void	Server::send_chan(const w_fd& fd, const std::string& chan, const std::string& str) {
	try {
		Client	client = get_client(fd);
		w_map_Channel::iterator it_channel = _channel.find(chan);

		if (it_channel == _channel.end()) {
			client.send_to_fd(W_ERR_NOSUCHNICK(client, chan, _name));
			return ;
		}
		it_channel->second.cast_send(PRIV_MSG(client, chan, str), client);
	} catch (std::exception& err) {
		std::cerr << "catch: " << err.what() << std::endl;
		return ;
	}
}
void	Server::send_priv(const w_fd& fd, const std::string& priv, const std::string& str) {
	try {
		Client	client = get_client(fd);

		w_map_Client::iterator it;
		for (it = _client.begin(); it != _client.end() && it->second.get_name() != priv; it++) ;
		if (it == _client.end()) {
			client.send_to_fd(W_ERR_NOSUCHNICK(client, priv, _name));
			return ;
		}

		it->second.send_to_fd(PRIV_MSG(client, priv, str));
	} catch (std::exception& err) {
		std::cerr << "catch: " << err.what() << std::endl;
		return ;
	}
}
void	Server::pong(const w_fd& fd, const std::string& token) {
	send_msg(fd, PONG_MSG(token));
}
void	Server::quit(const w_fd& fd, const std::string& str) {
	try {
		Client	client = get_client(fd);
		client.rm__to_map(_client);
		for (w_map_Channel::iterator it = _channel.begin(); it != _channel.end(); it++)
			it->second.quit(client, str);
	} catch (std::exception& err) {
		std::cerr << "catch: " << err.what() << std::endl;
		return ;
	}
}

void	Server::new_fd(const w_fd& socket) {
	w_pollfd	fd;

	fd.fd = socket;
	fd.events = POLLIN;
	fd.revents = 0;
	_fds.push_back(fd);
	w_vect_pollfd::iterator it = _fds.end() - 1;
	read(it);
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
	_channel.insert(w_pair_channel(channel, Channel(client, channel, _name)));
}

w_port		Server::get_port() const	{ return(_port); }
w_pass		Server::get_pass() const	{ return(_pass); }
std::string Server::get_name() const	{ return(_name); }


const Client&	Server::get_client(const w_fd& fd) const {
	w_map_Client::const_iterator it = _client.find(fd);
	if (it == _client.end())
		throw (std::runtime_error("Client Unknown"));
	return (it->second);
}
w_map_Client::iterator	Server::get_client(const std::string& name) {
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
