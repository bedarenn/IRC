#include "Channel.hpp"

Channel::Channel(const Client& client, const std::string& name, const std::string& server)
	: _name(name), _pass(""), _topic(name), _limit(100),
	_inv_only(false), _r_topic(false), _r_op(true),
	_server(server) {
	client.add_to_map(_client);
	client.add_to_map(_op);
	if (!_pass.empty())
		_r_pass = true;
}
Channel::~Channel() {}
Channel::Channel(const Channel& cpy) : _server(cpy._server) { *this = cpy; }

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
		return (join_pass(client));
	else if (client.is__in_map(_invite)) {
		client.rm__to_map(_invite);
		return (join_pass(client));
	}
	else if (_r_pass && pass == _pass)
		return (join_pass(client));
	return (false);
}
bool	Channel::invite(const Client& op, const Client& client) {
	if (_r_op && !op.is__in_map(_op))
		return (false);
	return (invite_pass(op, client));
}
bool	Channel::kick(const Client& op, const Client& client, const std::string& msg) {
	if (_r_op && !op.is__in_map(_op))
		return (false);
	return (kick_pass(op, client, msg));
}
bool	Channel::topic(const Client& op, const std::string& value) {
	if (_r_topic && !op.is__in_map(_op))
		return (false);
	return (topic_pass(op, value));
}

bool	Channel::join_pass(const Client& client) {
	if (!add_client(client))
		return (false);
	cast_send(JOIN_MSG(_name, client));
	if (!_topic.empty())
		client.send_to_fd(W_RPL_TOPIC(_name, client, _topic, _server));
	// RPL_TOPICWHOTIME
	send_list(client);
	return (true);
}
bool	Channel::invite_pass(const Client& op, const Client& client) {
	if (!client.add_to_map(_invite))
		return (false);
	client.send_to_fd(INVI_MSG(_name, op, client));
	op.send_to_fd(INVI_MSG(_name, op, client));
	op.send_to_fd(W_RPL_INVITING(op, client, _name, _server));
	return (true);
}
bool	Channel::kick_pass(const Client& op, const Client& client, std::string msg) {
	if (!rm__client(client))
		return (false);

	if (msg.empty())
		msg = KICK_MSG_DEFAULT;
	std::string	str(KICK_MSG(_name, op, client, msg));
	cast_send(str);
	client.send_to_fd(str);
	return (true);
}
bool	Channel::topic_pass(const Client& op, const std::string& value) {
	_topic = value;
	if (!_topic.empty())
		cast_send(W_RPL_TOPIC(_name, op, value, _server));
	else
		cast_send(W_RPL_NOTOPIC(_name, op, _server));
	// RPL_TOPICWHOTIME
	return (true);
}

bool	Channel::add_client(const Client& client) {
	return (client.add_to_map(_client));
}
bool	Channel::rm__client(const Client& client) {
	return (client.rm__to_map(_client) || client.rm__to_map(_op));
}
void	Channel::cast_send(const std::string& str) {
	for (w_map_Client::iterator it = _client.begin(); it != _client.end(); it++) {
		it->second.send_to_fd(str);
	}
}
void	Channel::cast_send(const std::string& str, const Client& client) {
	for (w_map_Client::iterator it = _client.begin(); it != _client.end(); it++) {
		if (it->second != client)
			it->second.send_to_fd(str);
	}
}
void	Channel::send_list(const Client& client) {
	for (w_map_Client::iterator it = _client.begin(); it != _client.end(); it++) {
		client.send_to_fd(W_RPL_NAMREPLY(_name, client, it->second, _server));
	}
	client.send_to_fd(W_RPL_ENDOFNAMES(_name, client, _server));
}

const std::string&	Channel::get_topic() const { return (_topic); }
void	Channel::set_topic(const std::string& value) { _topic = value; }
const std::string&	Channel::get_name() const { return (_name); }
void	Channel::set_name(const std::string& value) { _name = value; }

bool	Channel::empty() {
	return (_client.empty());
}
