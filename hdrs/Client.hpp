#pragma once

# include "types/Client.hpp"

class Server;
class Channel;

class Client {
public:
	Client(const std::string& name, const std::string& nickname, const w_fd& fd);
	~Client();
	Client(const Client& cpy);

	Client&	operator=(const Client& cpy);
	bool	operator==(const Client& cpy);
	bool	operator!=(const Client& cpy);

	bool	is__in_map(const w_map_Client& map_client) const;
	bool	add_to_map(w_map_Client& map_client) const;
	bool	rm__to_map(w_map_Client& map_client) const;

	ssize_t	send_to_fd(const std::string& str) const;

	const std::string&		get_name() const;
	const std::string&		get_nickname() const;
	const w_fd&				get_fd() const;

	friend std::ostream&	operator<<(std::ostream& out, const Client& client);

private:
	std::string		_name;
	std::string		_nickname;
	w_fd			_fd;

	Client();
};

ssize_t	send_msg(const w_fd& fd, const std::string& str);
