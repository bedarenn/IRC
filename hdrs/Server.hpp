#pragma once

# include "types/Server.hpp"

# include "Client.hpp"

class Server {
public:
	Server(w_port port, w_pass pass);
	~Server();
	Server(const Server& cpy);

	Server&	operator=(const Server& cpy);

	void	init_server();
	void	run();
	void	event();

	w_port		get_port() const;
	w_pass		get_pass() const;

private:
	w_port		_port;
	w_pass		_pass;

	Client		_client;

	Server();
};
