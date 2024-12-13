#include "Channel.hpp"

#include <sstream>
#include <algorithm>

#include <cstdlib>
#include <regex.h>

#define REGEX_INT	"^\\+?[1-9][0-9]*$"

Channel::Channel(Client *client, const std::string& name, const std::string& server)
	: _name(name), _pass(""), _limit(0),
	_inv_only(false), _r_topic(false), _r_pass(false), _r_limit(false),
	_topic(""),
	_server(server) {
	client->add_to_map(_op);
	join_pass(client);
	if (!_pass.empty())
		_r_pass = true;
}
Channel::~Channel() {}
Channel::Channel(const Channel& cpy) { *this = cpy; }

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
	_r_pass = cpy._r_pass;
	_r_limit = cpy._r_limit;

	_topic = cpy._topic;

	_server = cpy._server;

	return (*this);
}

bool	Channel::join(Client *client, const std::string& pass) {
	if (_inv_only) {
		if (is__invite(client->get_nickname())) {
			del_invite(client->get_nickname());
			return (join_pass(client));
		}
		else
			client->send_to_fd(W_ERR_INVITEONLYCHAN(client, _name, _server));
		return (false);
	}
	if (_r_pass) {
		if (pass == _pass)
			return (join_pass(client));
		else
			client->send_to_fd(W_ERR_BADCHANNELKEY(client, _name, _server));
		return (false);
	}
	return (join_pass(client));
}
bool	Channel::invite(Client *op, const std::string& client) {
	if (!op->is__in_map(_op)) {
		op->send_to_fd(W_ERR_CHANOPRIVSNEEDED(op, _name, _server));
		return (false);
	}
	return (invite_pass(op, client));
}
bool	Channel::kick(Client *op, Client *client, const std::string& msg) {
	if (!op->is__in_map(_client)) {
		op->send_to_fd(W_ERR_NOTONCHANNEL(op, _name, _server));
		return (false);
	}
	if (!op->is__in_map(_op)) {
		op->send_to_fd(W_ERR_CHANOPRIVSNEEDED(op, _name, _server));
		return (false);
	}
	return (kick_pass(op, client, msg));
}
bool	Channel::topic(Client *op, const std::string& value) {
	if (value.empty()) {
		send_topic(op);
		return (true);
	}
	if (!op->is__in_map(_client)) {
		op->send_to_fd(W_ERR_NOTONCHANNEL(op, _name, _server));
		return (false);
	}
	if (_r_topic && !op->is__in_map(_op)) {
		op->send_to_fd(W_ERR_CHANOPRIVSNEEDED(op, _name, _server));
		return (false);
	}
	return (topic_pass(value));
}
bool	Channel::mode(Client *op, const std::string& md, const std::string& arg) {
	if (!op->is__in_map(_client)) {
		op->send_to_fd(W_ERR_NOTONCHANNEL(op, _name, _server));
		return (false);
	}
	if (md.empty()) {
		return (mode_empty(op));
	}
	if (!op->is__in_map(_op)) {
		op->send_to_fd(W_ERR_CHANOPRIVSNEEDED(op, _name, _server));
		return (false);
	}
	return (mode_pass(op, md, arg));
}
bool	Channel::send(Client *client, const std::string& str) const {
	if (!client->is__in_map(_client)) {
		client->send_to_fd(W_ERR_CANNOTSENDTOCHAN(client, _name, _server));
		return (false);
	}
	if (client->is__in_map(_op))
		cast_send(PRIV_MSG_OP(client, _name, str), client);
	else
		cast_send(PRIV_MSG(client, _name, str), client);
	return (true);
}
bool	Channel::part(Client *client, const std::string& str) {
	if (!is_on_channel(client)) {
		client->send_to_fd(W_ERR_NOTONCHANNEL(client, _name, _server));
		return (false);
	}
	if (rm__client(client)) {
		std::string	s;
		if (str.empty())
			s = PART_MSG(_name, client);
		else
			s = PART_MSG_MSG(_name, client, str);
		cast_send(s);
		client->send_to_fd(s);
		return (true);
	}
	return (false);
}
bool	Channel::quit(Client *client, const std::string& str) {
	if (rm__client(client)) {
		if (str.empty())
			cast_send(QUIT_MSG(client));
		else
			cast_send(QUIT_MSG_MSG(client, str));
		return (true);
	}
	return (false);
}

bool	Channel::join_pass(Client *client) {
	if (_r_limit && _client.size() >= _limit) {
		client->send_to_fd(W_ERR_CHANNELISFULL(client, _name, _server));
		return (false);
	}
	if (!add_client(client))
		return (false);
	cast_send(JOIN_MSG(_name, client));
	send_topic(client);
	send_list(client);
	return (true);
}
bool	Channel::invite_pass(Client *op, const std::string& client) {
	if (!op->is__in_map(_client)) {
		op->send_to_fd(W_ERR_NOTONCHANNEL(op, _name, _server));
		return (false);
	}
	if (!op->is__in_map(_op)) {
		op->send_to_fd(W_ERR_CHANOPRIVSNEEDED(op, _name, _server));
		return (false);
	}
	if (is_on_channel(client)) {
		op->send_to_fd(W_ERR_USERONCHANNEL(op, client, _name, _server));
		return (false);
	}
	w_vect_invite::iterator	it;
	for (it = _invite.begin(); it != _invite.end() && *it != client; it++) ;
	if (it != _invite.end())
		return (false);
	_invite.push_back(client);
	return (true);
}
bool	Channel::kick_pass(Client *op, Client *client, const std::string& msg) {
	if (!rm__client(client)) {
		op->send_to_fd(W_ERR_USERNOTINCHANNEL(op, _name, client->get_nickname(), _server));
		return (false);
	}

	std::string	str;
	if (msg.empty())
		str = KICK_MSG(_name, op, client);
	else
		str = KICK_MSG_MSG(_name, op, client, msg);
	cast_send(str);
	client->send_to_fd(str);
	return (true);
}
bool	Channel::topic_pass(const std::string& value) {
	_topic = value;
	cast_f(&Channel::send_topic);
	return (true);
}
bool	Channel::mode_pass(Client *op, const std::string& md, const std::string& arg) {
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
		return (false);
	}
}

bool	Channel::add_client(Client *client) {
	return (client->add_to_map(_client));
}
bool	Channel::rm__client(Client *client) {
	return (client->rm__to_map(_client) || client->rm__to_map(_op));
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

bool	Channel::mode_i(Client *op, const std::string& md) {
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
bool	Channel::mode_t(Client *op, const std::string& md) {
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
bool	Channel::mode_k(Client *op, const std::string& md, const std::string& arg) {
	switch (md[0]) {
	case '+':
		if (_r_pass == true)
			return (false);
		if (arg.empty()) {
			op->send_to_fd(W_ERR_INVALIDMODEPARAM(op, _name, "+k", arg, "Not enough parameters", _server));
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
bool	Channel::mode_o(Client *op, const std::string& md, const std::string& arg) {
	w_map_Client::iterator	it;
	switch (md[0]) {
	case '+':
		if (arg.empty()) {
			op->send_to_fd(W_ERR_INVALIDMODEPARAM(op, _name, "+o", arg, "Not enough parameters", _server));
			return (false);
		}
		for (it = _client.begin(); it != _client.end() && it->second->get_nickname() != arg; it++) ;
		if (it == _client.end()) {
			op->send_to_fd(W_ERR_INVALIDMODEPARAM(op, _name, "+o", arg, "They aren't on that channel", _server));
			return (false);
		}
		if (it->second->is__in_map(_op)) {
			op->send_to_fd(W_ERR_INVALIDMODEPARAM(op, _name, "+o", arg, "is already operator", _server));
			return (false);
		}
		it->second->add_to_map(_op);
		cast_send(MODE_MSG_ARG(_name, op, md, arg, _server));
		return (true);
	case '-':
		if (arg.empty()) {
			op->send_to_fd(W_ERR_INVALIDMODEPARAM(op, _name, "-o", arg, "Not enough parameters", _server));
			return (false);
		}
		for (it = _client.begin(); it != _client.end() && it->second->get_nickname() != arg; it++) ;
		if (it == _client.end()) {
			op->send_to_fd(W_ERR_INVALIDMODEPARAM(op, _name, "-o", arg, "They aren't on that channel", _server));
			return (false);
		}
		if (!it->second->is__in_map(_op)) {
			op->send_to_fd(W_ERR_INVALIDMODEPARAM(op, _name, "-o", arg, "is not operator", _server));
			return (false);
		}
		it->second->rm__to_map(_op);
		cast_send(MODE_MSG_ARG(_name, op, md, arg, _server));
		return (true);
	default:
		return (false);
	}
}
bool	Channel::mode_l(Client *op, const std::string& md, const std::string& arg) {
	bool cmp;
	regex_t	regex;

	switch (md[0]) {
	case '+':
		if (arg.empty()) {
			op->send_to_fd(W_ERR_INVALIDMODEPARAM(op, _name, "+l", arg, "Not enough parameters", _server));
			return (false);
		}
		regcomp(&regex, REGEX_INT, REG_EXTENDED);
		cmp = regexec(&regex, arg.c_str(), 0, NULL, 0);
		regfree(&regex);
		if (cmp) {
			op->send_to_fd(W_ERR_INVALIDMODEPARAM(op, _name, "+l", arg, "Not a number", _server));
			return (false);
		}
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
bool	Channel::mode_empty(Client *op) {
	std::stringstream	mod;
	std::stringstream	arg;

	if (_inv_only)
		mod << "i";
	if (_r_topic)
		mod << "t";
	if (_r_pass) {
		mod << "k";
		arg << _pass;
	}
	if (_r_limit) {
		mod << "l";
		if (_r_pass)
			arg << " ";
		arg << _limit;
	}
	if (mod.str().empty())
		op->send_to_fd(W_RPL_CHANNELMODEIS_EMPTY(op, _name, _server));
	else if (arg.str().empty())
		op->send_to_fd(W_RPL_CHANNELMODEIS(op, _name, "+" + mod.str(), _server));
	else
		op->send_to_fd(W_RPL_CHANNELMODEIS_ARG(op, _name, "+" + mod.str(), arg.str(), _server));
	return (true);
}

bool	Channel::is_on_channel(const w_fd& fd) const {
	w_map_Client::const_iterator it;
	for (it = _client.begin(); it != _client.end() && it->second->get_fd() != fd; it++) ;
	if (it == _client.end())
		return (false);
	return (true);
}
bool	Channel::is_on_channel(Client *client) const {
	w_map_Client::const_iterator it;
	for (it = _client.begin(); it != _client.end() && it->second != client; it++) ;
	if (it == _client.end())
		return (false);
	std::cout << "IS IT ?? " << it->second->get_fd() << "" << std::endl;
	return (true);
}
bool	Channel::is_on_channel(const std::string& nick) const {
	w_map_Client::const_iterator it;
	for (it = _client.begin(); it != _client.end() && it->second->get_nickname() != nick; it++) ;
	if (it == _client.end())
		return (false);
	std::cout << "IS IT ?? " << it->second->get_fd() << "" << std::endl;
	return (true);
}

void	Channel::cast_send(const std::string& str) const {
	for (w_map_Client::const_iterator it = _client.begin(); it != _client.end(); it++) {
		it->second->send_to_fd(str);
	}
}
void	Channel::cast_send(const std::string& str, Client *client) const {
	for (w_map_Client::const_iterator it = _client.begin(); it != _client.end(); it++) {
		if (it->second != client)
			it->second->send_to_fd(str);
	}
}
void	Channel::cast_f(void (Channel::*f)(Client *)) {
	for (w_map_Client::iterator it = _client.begin(); it != _client.end(); it++)
		(this->*f)(it->second);
}

void	Channel::send_topic(Client *client) {
	if (!_topic.empty())
		client->send_to_fd(W_RPL_TOPIC(client, _name, _topic, _server));
	else
		client->send_to_fd(W_RPL_NOTOPIC(client, _name, _server));
}
void	Channel::send_list(Client *client) {
	for (w_map_Client::iterator it = _client.begin(); it != _client.end(); it++) {
		if (it->second->is__in_map(_op))
			client->send_to_fd(W_RPL_NAMREPLY(client, _name, "@" + it->second->get_nickname(), _server));
		else
			client->send_to_fd(W_RPL_NAMREPLY(client, _name, it->second->get_nickname(), _server));
	}
	client->send_to_fd(W_RPL_ENDOFNAMES(client, _name, _server));
}

const std::string&	Channel::get_topic() const { return (_topic); }
void	Channel::set_topic(const std::string& value) { _topic = value; }
const std::string&	Channel::get_name() const { return (_name); }
void	Channel::set_name(const std::string& value) { _name = value; }

bool	Channel::empty() {
	return (_client.empty());
}
