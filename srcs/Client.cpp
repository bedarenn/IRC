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

const std::string&		Client::get_name() const { return (_name); }
const std::string&		Client::get_nickname() const { return (_nickname); }
const w_fd&				Client::get_fd() const { return (_fd); }
