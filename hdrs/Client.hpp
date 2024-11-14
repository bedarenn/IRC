#pragma once

# include "types/Client.hpp"

class Client {
public:
	Client();
	~Client();
	Client(const Client& cpy);

	Client&	operator=(const Client& cpy);

	void	add_new(const w_fd& socket);
	void	add_new(const w_pollfd& fd);
	void	event();

	void	connect();
	void	read(w_vect_pollfd::iterator& poll);
	void	write(std::string& str,
		const w_vect_pollfd& grp,
		const w_pollfd& clt, const w_pollfd& srv);

private:
	w_vect_pollfd	_fds;

	char	buff[BUFFER_SIZE];
};
