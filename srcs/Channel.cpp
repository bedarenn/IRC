#include "Channel.hpp"

Channel::Channel() {}
Channel::~Channel() {}
Channel::Channel(const Channel& cpy) { *this = cpy; }

Channel&	Channel::operator=(const Channel& cpy) {
	if (this == &cpy)
		return (*this);
	_client = cpy._client;
	_op = cpy._op;
	_inv_only = cpy._inv_only;
	_pass = cpy._pass;
	_limit = cpy._limit;
	return (*this);
}

void	Channel::rm__client(w_fd fd) {
	w_map_Client::iterator	it;
	it = _client.find(fd);
	if (it != _client.end())
		_client.erase(it);
	it = _op.find(fd);
	if (it != _op.end())
		_op.erase(it);
}
