#include "Client.hpp"
#include "Command.hpp"

#include <sys/socket.h>
#include <sstream>

Client::Client() {}
Client::Client(const w_fd& fd)
	: _fd(fd), _name(""), _nickname(""),
	_is_connect(false), _buff("") {}
Client::~Client() {}
Client::Client(const Client& cpy) { *this = cpy; }

Client&	Client::operator=(const Client& cpy) {
	if (this == &cpy)
		return (*this);
	_name = cpy._name;
	_nickname = cpy._nickname;
	_fd = cpy._fd;
	_is_connect = cpy._is_connect;
	_buff = cpy._buff;
	return (*this);
}
bool	Client::operator==(const Client& cpy) const { return (_fd == cpy._fd); }
bool	Client::operator!=(const Client& cpy) const { return (_fd != cpy._fd); }

bool	Client::is__in_map(const w_map_Client& map_client) const {
	w_map_Client::const_iterator	it = map_client.find(_fd);
	if (it == map_client.end())
		return (false);
	return (true);
}
bool	Client::add_to_map(w_map_Client& map_client) const {
	if (is__in_map(map_client))
		return (false);
	map_client.insert(w_pair_Client(_fd, *this));
	return (true);
}
bool	Client::rm__to_map(w_map_Client& map_client) const {
	w_map_Client::iterator	it = map_client.find(_fd);
	if (it == map_client.end())
		return (false);
	map_client.erase(it);
	return (true);
}

bool	Client::connect() {
	if (_is_connect == true)
		return (false);
	_is_connect = true;
	return (true);
}
ssize_t	Client::send_to_fd(const std::string& str) const {
	return (send_msg(_fd, str));
}
bool	Client::read_buff(const std::string& str, Server *server) {
	_buff = std::string(_buff + str);

	if (_buff.find('\n') != std::string::npos)
		exec_cmd(server);
	else
		return (false);
	return (true);
}
void	Client::exec_cmd(Server *server) {
	std::stringstream	sstream(_buff);
	std::string			line;

	while (std::getline(sstream, line)) {
		//std::cout << _fd << " << " << line << std::flush;
		Command(_fd, line, server);
	}
	_buff.clear();
}

const w_fd&			Client::get_fd() const { return (_fd); }
bool				Client::is_connect() const {
	if (_name.empty() || _nickname.empty())
		return (false);
	return (_is_connect);
}

const std::string&	Client::get_name() const { return (_name); }
const std::string&	Client::get_nickname() const { return (_nickname); }
bool				Client::set_name(const std::string& value) {
	if (!_is_connect)
		return (false);
	_name = value;
	return (true);
}
bool				Client::set_nickname(const std::string& value) {
	if (!_is_connect)
		return (false);
	_nickname = value;
	return (true);
}

std::ostream&	operator<<(std::ostream& out, const Client& client) {
	out << client._fd << ": " << client._nickname;
	return (out);
}

ssize_t	send_msg(const w_fd& fd, const std::string& str) {
	std::string s = W_SND(str);
	 << fd << " >> " << s << std::endl;
	return (send(fd, s.c_str(), s.size(), 0));
}