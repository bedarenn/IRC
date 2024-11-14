#pragma once

# include "types/Client.hpp"

class Client {
public:
	Client();
	~Client();
	Client(const Client& cpy);

	Client&	operator=(const Client& cpy);

	void	add_new(w_pollfd fd);
	void	event();

	void	connect();
	void	read(w_vect_pollfd::iterator& poll);

private:
	w_vect_pollfd	_fds;
	Command			_cmd;

	char	buff[BUFFER_SIZE];
};
