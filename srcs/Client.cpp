#include "Client.hpp"

Client::Client(const std::string& name, const w_fd& fd) : _name(name), _fd(fd) {}
Client::~Client() {}
Client::Client(const Client& cpy) { *this = cpy; }

Client&	Client::operator=(const Client& cpy) {
	if (this == &cpy)
		return (*this);
	_name = cpy._name;
	_fd = cpy._fd;
	_channel = cpy._channel;
	return (*this);
}
