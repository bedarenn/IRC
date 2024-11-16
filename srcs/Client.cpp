#include "Client.hpp"

#include <sys/socket.h>

Client::Client() {}
Client::Client(const std::string& name, const std::string& nickname, const w_fd& fd)
	: _name(name), _nickname(nickname), _fd(fd) {}
Client::~Client() {}
Client::Client(const Client& cpy) { *this = cpy; }

Client&	Client::operator=(const Client& cpy) {
	if (this == &cpy)
		return (*this);
	_name = cpy._name;
	_nickname = cpy._nickname;
	_fd = cpy._fd;
	return (*this);
}

bool	Client::is__in_map(const w_map_Client& map_client) const {
	w_map_Client::const_iterator	it = map_client.find(_fd);
	if (it == map_client.end())
		return (false);
	return (true);
}
bool	Client::add_to_map(w_map_Client& map_client) const {
	if (is__in_map(map_client))
		return (false);
	map_client.insert(w_pair_Client(_fd, *this));
	return (true);
}
bool	Client::rm__to_map(w_map_Client& map_client) const {
	w_map_Client::iterator	it = map_client.find(_fd);
	if (it == map_client.end())
		return (false);
	map_client.erase(it);
	return (true);
}

ssize_t	Client::send_to_fd(const std::string& str) const {
	return (send(_fd, str.c_str(), str.size(), 0));
}

const std::string&		Client::get_name() const { return (_name); }
const std::string&		Client::get_nickname() const { return (_nickname); }
const w_fd&				Client::get_fd() const { return (_fd); }
