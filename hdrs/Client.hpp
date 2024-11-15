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

	bool	add_to_map(w_map_Client map_client) const;
	bool	rm__to_map(w_map_Client map_client);
	ssize_t	send_to_fd(std::string str);

	const std::string&		get_name() const;
	const std::string&		get_nickname() const;
	const w_fd&				get_fd() const;

private:
	std::string		_name;
	std::string		_nickname;
	w_fd			_fd;

	Client();
};
