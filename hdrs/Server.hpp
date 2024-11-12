#pragma once

# include "types/Server.hpp"

# include <iostream>

extern int g_loop;

class Server {
public:
	Server(w_port port, w_pass pass);
	~Server();
	Server(const Server& cpy);

	Server&	operator=(const Server& cpy);

	void		init_server();
	void		run();
	void		add_new(int socket);
	void		connect();
	void		received_data(size_t fd);
	void		clear_fd();
	void		close_client(int fd);

	w_port		get_port() const;
	w_pass		get_pass() const;
	int			get_fd_serv() const;
	void		set_port(w_port port);
	void		set_pass(w_pass pass);

private:
	w_port		_port;
	w_pass		_pass;

	w_fd			_server_fd;
	w_vect_pollfd	_fds;

	Server();
};
