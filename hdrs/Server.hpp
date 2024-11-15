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

	void	connect();
	void	read(w_vect_pollfd::iterator& poll);

	void	new_fd(const w_fd& socket);
	void	new_client(const std::string& name, const std::string& nickname, const w_fd& fd);
	void	rm__client(const Client& client);

	w_port		get_port() const;
	w_pass		get_pass() const;

	const Client&	get_client(w_fd fd) const;

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
