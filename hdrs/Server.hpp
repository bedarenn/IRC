#pragma once

# include "types/Server.hpp"

# include "Channel.hpp"

class Server {
public:
	Server(w_port port, w_pass pass);
	~Server();
	Server(const Server& cpy);

	Server&	operator=(const Server& cpy);

	void	init_server();

	void	run();
	void	event();

	void	treatement(int fd, char *buff);

	void	connect();
	void	add_new(const w_fd& socket);
	void	add_new(const w_pollfd& fd);

	void	read(w_vect_pollfd::iterator& poll);

	w_port		get_port() const;
	w_pass		get_pass() const;

private:
	w_port		_port;
	w_pass		_pass;

	w_vect_pollfd	_fds;
	w_map_Client	_client;
	w_channel		_channel;
	std::map<std::string, void(*)(std::string, int)>	_cmd;

	char	buff[BUFFER_SIZE];

	Server();
};
