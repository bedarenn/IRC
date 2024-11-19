#include "Channel.hpp"

Channel::Channel() {}
Channel::Channel(const Client& client, const std::string& name)
	: _name(name), _pass(""), _topic(name), _limit(100),
	_inv_only(false), _r_topic(false), _r_op(true) {
	client.add_to_map(_client);
	client.add_to_map(_op);
	if (!_pass.empty())
		_r_pass = true;
}
Channel::~Channel() {}
Channel::Channel(const Channel& cpy) { *this = cpy; }

Channel&	Channel::operator=(const Channel& cpy) {
	if (this == &cpy)
		return (*this);
	_client = cpy._client;
	_name = cpy._name;
	_topic = cpy._topic;
	_op = cpy._op;
	_inv_only = cpy._inv_only;
	_r_topic = cpy._r_topic;
	_pass = cpy._pass;
	_r_op = cpy._r_op;
	_limit = cpy._limit;
	return (*this);
}

bool	Channel::join(const Client& client, const std::string& pass) {
	if (!_r_pass)
		add_client(client);
	else if (client.is__in_map(_invite)) {
		client.rm__to_map(_invite);
		add_client(client);
	}
	else if (_r_pass && pass == _pass)
		add_client(client);
	else
		return (false);
	return (true);
}
bool	Channel::invite(const Client& op, const Client& client) {
	if (_r_op && !op.is__in_map(_op))
		return (false);
	return (client.add_to_map(_invite));
}
bool	Channel::kick(const Client& op, const Client& client) {
	if (_r_op && !op.is__in_map(_op))
		return (false);
	return (rm__client(client));
}
bool	Channel::topic(const Client& op, const std::string& value) {
	if (_r_topic && !op.is__in_map(_op))
		return (false);
	_topic = value;
	return (true);
}

bool	Channel::add_client(const Client& client) {
	return (client.add_to_map(_client));
}
bool	Channel::rm__client(const Client& client) {
	return (client.rm__to_map(_client) || client.rm__to_map(_op));
}

const std::string&	Channel::get_topic() const { return (_topic); }
void	Channel::set_topic(const std::string& value) { _topic = value; }

bool	Channel::empty() {
	return (_client.empty());
}
