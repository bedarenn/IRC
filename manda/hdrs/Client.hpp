#pragma once

# include "types/Client.hpp"

class Server;
class Channel;

class Client {
public:
	Client(const w_fd& fd);
	~Client();
	Client(const Client& cpy);

	Client&	operator=(const Client& cpy);
	bool	operator!=(const std::string& nick);
	bool	operator==(const std::string& nick);

	bool	is__in_map(const w_map_Client& map_client) const;
	bool	add_to_map(w_map_Client& map_client);
	bool	rm__to_map(w_map_Client& map_client) const;

	bool	connect();
	bool	read_buff(const std::string& str);
	void	exec_cmd(Server *server);
	ssize_t	send_to_fd(const std::string& str) const;

	const w_fd&			get_fd() const;
	bool				is_connect() const;

	const std::string&	get_name() const;
	const std::string&	get_nickname() const;
	const std::string&	get_buff() const;
	bool				set_name(const std::string& value);
	bool				set_nickname(const std::string& value);
	void				clear_buff();

	friend std::ostream&	operator<<(std::ostream& out, const Client& client);

private:
	w_fd		_fd;

	std::string	_name;
	std::string	_nickname;

	bool		_is_connect;
	std::string	_buff;

	Client();
};

ssize_t	send_msg(const w_fd& fd, const std::string& str);
