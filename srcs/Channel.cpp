#include "Channel.hpp"

Channel::Channel() {}
Channel::Channel(const std::string& name, const Client& client) : _name(name) {
	client.add_to_map(_client);
	client.add_to_map(_op);
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
	_topic_right = cpy._topic_right;
	_pass = cpy._pass;
	_op_right = cpy._op_right;
	_limit = cpy._limit;
	return (*this);
}

void	Channel::add_client(const Client& client) {
	client.add_to_map(_client);
}

void	Channel::rm__client(const Client& client) {
	client.rm__to_map(_client);
	client.rm__to_map(_op);
}
void	Channel::inv_client(const Client& op, const Client& client) {
	if (!op.is__in_map(_op))
		return ;
	add_client(client);
}

const std::string&	Channel::get_topic() const { return (_topic); }
void	Channel::set_topic(const std::string& value) { _topic = value; }

bool	Channel::empty() {
	return (_client.empty());
}
