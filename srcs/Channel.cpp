#include "Channel.hpp"

#include <sstream>
#include <algorithm>

#include <cstdlib>
#include <regex.h>

#define REGEX_INT	"^[+]?[0-9]+$"

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
bool	Channel::mode(const Client& op, const std::string& md, const std::string& arg) {
	if (!op.is__in_map(_client)) {
		op.send_to_fd(W_ERR_NOTONCHANNEL(op, "MODE", _server));
		return (false);
	}
	if (md.empty()) {
		return (true);
	}
	if (!op.is__in_map(_op)) {
		op.send_to_fd(W_ERR_CHANOPRIVSNEEDED(op, "MODE", _server));
		return (false);
	}
	return (mode_pass(op, md, arg));
}
bool	Channel::quit(const Client& client, const std::string& str) {
	if (rm__client(client)) {
		cast_send(QUIT_MSG(str));
		return (true);
	}
	return (false);
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
bool	Channel::mode_pass(const Client& op, const std::string& md, const std::string& arg) {
	switch (md[1]) {
	case 'i':
		return (mode_i(op, md));
	case 't':
		return (mode_t(op, md));
	case 'k':
		return (mode_k(op, md, arg));
	case 'o':
		return (mode_o(op, md, arg));
	case 'l':
		return (mode_l(op, md, arg));
	default:
		return (mode_empty(op));
	}
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

bool	Channel::mode_i(const Client& op, const std::string& md) {
	switch (md[0]) {
	case '+':
		if (_inv_only == true)
			return (false);
		_inv_only = true;
		cast_send(MODE_MSG(_name, op, md, _server));
		return (true);
	case '-':
		if (_inv_only == false)
			return (false);
		_inv_only = false;
		cast_send(MODE_MSG(_name, op, md, _server));
		return (true);
	default:
		return (false);
	}
}
bool	Channel::mode_t(const Client& op, const std::string& md) {
	switch (md[0]) {
	case '+':
		if (_r_topic == true)
			return (false);
		_r_topic = true;
		cast_send(MODE_MSG(_name, op, md, _server));
		return (true);
	case '-':
		if (_r_topic == false)
			return (false);
		_r_topic = false;
		cast_send(MODE_MSG(_name, op, md, _server));
		return (true);
	default:
		return (false);
	}
}
bool	Channel::mode_k(const Client& op, const std::string& md, const std::string& arg) {
	switch (md[0]) {
	case '+':
		if (_r_pass == true)
			return (false);
		if (arg.empty()) {
			op.send_to_fd(W_ERR_NEEDMOREPARAMS(op, "MODE", _server));
			return (false);
		}
		_pass = arg;
		_r_pass = true;
		cast_send(MODE_MSG_ARG(_name, op, md, arg, _server));
		return (true);
	case '-':
		if (_r_pass == false)
			return (false);
		_r_pass = false;
		cast_send(MODE_MSG(_name, op, md, _server));
		return (true);
	default:
		return (false);
	}
}
bool	Channel::mode_o(const Client& op, const std::string& md, const std::string& arg) {
	w_map_Client::iterator	it;
	switch (md[0]) {
	case '+':
		if (arg.empty()) {
			op.send_to_fd(W_ERR_NEEDMOREPARAMS(op, "MODE", _server));
			return (false);
		}
		for (it = _op.begin(); it != _op.end(); it++) {
			if (arg == it->second.get_nickname())
				return (false);
		}
		for (it = _client.begin(); it != _client.end() && it->second.get_nickname() == arg; it++) ;
		if (it == _client.end() || it->second.is__in_map(_op))
			return (false);
		it->second.add_to_map(_op);
		cast_send(MODE_MSG_ARG(_name, op, md, arg, _server));
		return (true);
	case '-':
		if (arg.empty()) {
			op.send_to_fd(W_ERR_NEEDMOREPARAMS(op, "MODE", _server));
			return (false);
		}
		for (it = _op.begin(); it != _op.end() && it->second.get_nickname() != arg; it++) ;
		if (it != _op.end())
			return (false);
		for (it = _client.begin(); it != _client.end() && it->second.get_nickname() == arg; it++) ;
		if (it == _client.end() || !it->second.is__in_map(_op))
			return (false);
		it->second.rm__to_map(_op);
		cast_send(MODE_MSG_ARG(_name, op, md, arg, _server));
		return (true);
	default:
		return (false);
	}
}
bool	Channel::mode_l(const Client& op, const std::string& md, const std::string& arg) {
	switch (md[0]) {
	case '+':
		if (_r_limit == true)
			return (false);
		if (arg.empty()) {
			op.send_to_fd(W_ERR_NEEDMOREPARAMS(op, "MODE", _server));
			return (false);
		}
		regex_t	regex;
		regcomp(&regex, REGEX_INT, REG_EXTENDED);
		if (!regexec(&regex, arg.c_str(), 0, NULL, 0)) {
			regfree(&regex);
			return (false);
		}
		regfree(&regex);
		_limit = atoi(arg.c_str());
		_r_limit = true;
		cast_send(MODE_MSG_ARG(_name, op, md, arg, _server));
		return (true);
	case '-':
		if (_r_limit == false)
			return (false);
		_r_limit = false;
		cast_send(MODE_MSG(_name, op, md, _server));
		return (true);
	default:
		return (false);
	}
}
bool	Channel::mode_empty(const Client& op) {
	std::stringstream	str;
	std::stringstream	size;

	if (_inv_only == true)
		str << "i";
	if (_r_topic == true)
		str << "t";
	if (_r_op == true)
		str << "k";
	if (_inv_only == true)
		str << "i";
	if (_r_limit == true) {
		str << "i";
		size << _limit;
		op.send_to_fd(W_RPL_CHANNELMODEIS_ARG(op, _name, "+" + str.str(), size.str(), _server));
	}
	else if (!str.str().empty())
		op.send_to_fd(W_RPL_CHANNELMODEIS(op, _name, "+" + str.str(), _server));
	else
		op.send_to_fd(W_RPL_CHANNELMODEIS_EMPTY(op, _name, _server));
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
		//client.send_to_fd(W_RPL_TOPICWHOTIME(_name, _topic_change, get_topic_time(), _server));
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
