#include "Channel.hpp"

#include <sstream>
#include <algorithm>

Channel::Channel(const Client& client, const std::string& name, const std::string& server)
	: _name(name), _pass(""), _limit(0),
	_inv_only(false), _r_topic(false), _r_op(true), _r_pass(false), _r_limit(false),
	_topic(name), _topic_time(std::time(0)), _topic_change(client),
	_server(server) {
	join_pass(client);
	client.add_to_map(_op);
	if (!_pass.empty())
		_r_pass = true;
}
Channel::~Channel() {}
Channel::Channel(const Channel& cpy) : _topic_change(cpy._topic_change) { *this = cpy; }

Channel&	Channel::operator=(const Channel& cpy) {
	if (this == &cpy)
		return (*this);

	_client = cpy._client;
	_invite = cpy._invite;
	_op = cpy._op;

	_name = cpy._name;
	_pass = cpy._pass;
	_limit = cpy._limit;

	_inv_only = cpy._inv_only;
	_r_topic = cpy._r_topic;
	_r_op = cpy._r_op;
	_r_pass = cpy._r_pass;
	_r_limit = cpy._r_limit;

	_topic = cpy._topic;
	_topic_time = cpy._topic_time;
	_topic_change = cpy._topic_change;

	_server = cpy._server;

	return (*this);
}

bool	Channel::join(const Client& client, const std::string& pass) {
	if (_inv_only) {
		if (is__invite(client.get_nickname())) {
			del_invite(client.get_nickname());
			return (join_pass(client));
		}
		else
			client.send_to_fd(W_ERR_INVITEONLYCHAN(client, "JOIN", _server));
		return (false);
	}
	if (_r_pass) {
		if (pass == _pass)
			return (join_pass(client));
		else
			client.send_to_fd(W_ERR_BADCHANNELKEY(client, "JOIN", _server));
		return (false);
	}
	return (join_pass(client));
}
bool	Channel::invite(const Client& op, const std::string& client) {
	if (_r_op && !op.is__in_map(_op))
		return (false);
	return (invite_pass(op, client));
}
bool	Channel::kick(const Client& op, const Client& client, const std::string& msg) {
	if (_r_op && !op.is__in_map(_op)) {
		op.send_to_fd(W_ERR_CHANOPRIVSNEEDED(op, "KICK", _server));
		return (false);
	}
	return (kick_pass(op, client, msg));
}
bool	Channel::topic(const Client& op, const std::string& value) {
	if (!op.is__in_map(_client)) {
		op.send_to_fd(W_ERR_NOTONCHANNEL(op, "TOPIC", _server));
		return (false);
	}
	if (_r_topic && !op.is__in_map(_op)) {
		op.send_to_fd(W_ERR_CHANOPRIVSNEEDED(op, "TOPIC", _server));
		return (false);
	}
	return (topic_pass(op, value));
}

bool	Channel::join_pass(const Client& client) {
	if (_r_limit && _client.size() >= _limit) {
		client.send_to_fd(W_ERR_CHANNELISFULL(client, "JOIN", _server));
		return (false);
	}
	if (!add_client(client))
		return (false);
	cast_send(JOIN_MSG(_name, client));
	send_topic(client);
	send_list(client);
	return (true);
}
bool	Channel::invite_pass(const Client& op, const std::string& client) {
	if (!op.is__in_map(_client)) {
		op.send_to_fd(W_ERR_NOTONCHANNEL(op, "INVITE", _server));
		return (false);
	}
	if (!op.is__in_map(_op)) {
		op.send_to_fd(W_ERR_CHANOPRIVSNEEDED(op, "INVITE", _server));
		return (false);
	}
	if (is_on_channel(client)) {
		op.send_to_fd(W_ERR_CHANOPRIVSNEEDED(op, "INVITE", _server));
		return (false);
	}
	_invite.push_back(client);
	return (true);
}
bool	Channel::kick_pass(const Client& op, const Client& client, std::string msg) {
	if (!rm__client(client)) {
		op.send_to_fd(W_ERR_NOTONCHANNEL(op, "KICK", _server));
		return (false);
	}

	if (msg.empty())
		msg = KICK_MSG_DEFAULT;
	std::string	str(KICK_MSG(_name, op, client, msg));
	cast_send(str);
	client.send_to_fd(str);
	return (true);
}
bool	Channel::topic_pass(const Client& op, const std::string& value) {
	_topic = value;
	_topic_time = std::time(0);
	_topic_change = op;
	cast_f(&Channel::send_topic);
	return (true);
}

bool	Channel::add_client(const Client& client) {
	return (client.add_to_map(_client));
}
bool	Channel::rm__client(const Client& client) {
	return (client.rm__to_map(_client) || client.rm__to_map(_op));
}

bool	Channel::is__invite(const std::string& client) const {	
	if (std::find(_invite.begin(), _invite.end(), client) == _invite.end())
		return (false);
	return (true);
}
bool	Channel::del_invite(const std::string& client) {
	w_vect_invite::iterator	it = std::find(_invite.begin(), _invite.end(), client);
	if (it == _invite.end())
		return (false);
	_invite.erase(it);
	return (true);
}

bool	Channel::is_on_channel(const std::string& client) const {
	for (w_map_Client::const_iterator it = _client.begin(); it != _client.end(); it++) {
		if (it->second.get_nickname() == client)
			return (true);
	}
	return (false);
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
void	Channel::cast_f(void (Channel::*f)(const Client&)) {
	for (w_map_Client::iterator it = _client.begin(); it != _client.end(); it++)
		(this->*f)(it->second);
}

void	Channel::send_topic(const Client& client) {
	if (!_topic.empty()) {
		client.send_to_fd(W_RPL_TOPIC(_name, client, _topic, _server));
		client.send_to_fd(W_RPL_TOPICWHOTIME(_name, _topic_change, get_topic_time(), _server));
	}
	else {
		cast_send(W_RPL_NOTOPIC(_name, client, _server));
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

std::string	Channel::get_topic_time() const {
	std::ostringstream	oss;
	oss << _topic_time;

	return (oss.str());
}

bool	Channel::empty() {
	return (_client.empty());
}
