#pragma once

# include "types/Client.hpp"

class Client {
public:
	Client(const std::string& name, const w_fd& fd);
	~Client();
	Client(const Client& cpy);

	Client&	operator=(const Client& cpy);

private:
	std::string		_name;
	std::string		_nickname;
	w_fd			_fd;
	w_channel_list	_channel;

	Client();
};
