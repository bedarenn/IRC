#include "Client.hpp"

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

bool	Client::add_to_map(w_map_Client map_client) const {
	w_map_Client::iterator	it = map_client.find(_fd);
	if (it == map_client.end())
		return (false);
	map_client.insert(w_pair_Client(_fd, *this));
	return (true);
}

const std::string&		Client::get_name() const { return (_name); }
const std::string&		Client::get_nickname() const { return (_nickname); }
const w_fd&				Client::get_fd() const { return (_fd); }
